#include "ThreadWorker.h"

ThreadWorker::Node::Node() :
	next(nullptr)
{
}

ThreadWorker::Node::Node(const Callable& func) :
	next(nullptr),
	func(func)
{
}

const int ThreadWorker::PRE_START = 1;
const int ThreadWorker::RUNNING = 2;
const int ThreadWorker::AWAITING_TERMINATE = 3;
const int ThreadWorker::TERMINATED = 4;
const ThreadWorker::Callable ThreadWorker::TERMINATED_SIGNAL = []() {};
const SimpleLogger ThreadWorker::LOG = SimpleLogger("ThreadWorker");

ThreadWorker::ThreadWorker(uint32_t capacity, std::chrono::milliseconds timeoutMillis) :
	capacity(capacity),
	state(PRE_START),
	size(0),
	timeoutMillis(timeoutMillis),
	readMutex(),
	writeMutex(),
	notEmpty(),
	notFull()
{
	tail = head = new Node();
}

ThreadWorker::~ThreadWorker()
{
	TerminateNow();
}

ThreadWorker::ThreadWorker(ThreadWorker&& other) noexcept :
	ThreadWorker(other.capacity)
{
	MoveFrom(other);
}

ThreadWorker& ThreadWorker::operator=(ThreadWorker&& other) noexcept
{
	MoveFrom(other);

	return *this;
}

bool ThreadWorker::Start()
{
	LOG << info() << "Trying to start the worker";
	int s = PRE_START;
	if (state.compare_exchange_strong(s, RUNNING))
	{
		LOG << debug() << "Spawning the worker thread";
		// Start waiting for and consuming tasks
		if (thread.joinable()) // We might be joinable if timed out while waiting for work
		{
			thread.join();
		}
		thread = std::thread([this]() { RunWorker(); });

		return true;
	}

	return false;
}

void ThreadWorker::TerminateNow()
{
	LOG << info() << "Trying to terminate worker";

	// TODO: CASing to TERMINATE while Submitting 
	// may cause tasks that are gonna be ignored 
	// by the work loop to be submitted needlesly
	int s = PRE_START;
	if (!state.compare_exchange_strong(s, TERMINATED) &&
		s == RUNNING && // Else we are already terminated (or being terminated)
		state.compare_exchange_strong(s, TERMINATED))
	{
		// The worker is running, clear and push a TERMINATE_SIGNAL
		std::unique_lock<std::recursive_mutex> readLock(readMutex, std::defer_lock),
			writeLock(writeMutex, std::defer_lock);
		std::lock(readLock, writeLock);
		{
			LOG << debug() << "CASed to TERMINATE";

			// Clear queue, no further submission can occur at this point
			Clear();

			// Push a dummy signal in case the wokrer is waiting
			Enqueue(TERMINATED_SIGNAL);
			size++;
			// Notify worker that we pushed a TERMINATE_SIGNAL
			notEmpty.notify_all();
		}
	}
	// Else we are already terminated

	// Wait for the thread to join if joinable
	if (thread.joinable())
	{
		LOG << debug() << "Awaiting join...";

		thread.join();
	}
	else
	{
		LOG << warn() << "Non joinable thread on state: " << state.load();
	}
}

void ThreadWorker::AwaitTermination()
{
	LOG << info() << "Awaiting to terminate...";

	int s = PRE_START;
	if (!state.compare_exchange_strong(s, TERMINATED) && // If PRE_START then just TERMINATE directly
		s == RUNNING && // Else we are already terminated
		state.compare_exchange_strong(s, AWAITING_TERMINATE)) // Only AWAIT_TERMINATE if RUNNING
	{
		std::unique_lock<std::recursive_mutex> writeLock(writeMutex);
		{
			LOG << debug() << "CASed to AWAITING_TERMINATE";

			////////////////////////////////////////////////////////////////////////
			// Don't clear the queue and let the worker finish all submitted work //
			////////////////////////////////////////////////////////////////////////


			// Wait till space is available
			while (size.load() == capacity)
			{
				notFull.wait(writeLock);
			}
			// Push a dummy signal in case the wokrer is waiting
			Enqueue(TERMINATED_SIGNAL);
			size_t s = size++;
			if (s == 0)
			{
				// Another thread might be waiting
				// for not emtpy, so we signal them
				SignalNotEmpty();
			}
		}
	}

	do
	{
		s = AWAITING_TERMINATE;

		if (thread.joinable())
		{
			LOG << debug() << "Awaiting join...";

			thread.join();
		}
		else
		{
			LOG << debug() << "Non joinable thread on state: " << state.load();
		}
	} while (!state.compare_exchange_strong(s, TERMINATED));
}

bool ThreadWorker::IsStarted() const
{
	return state.load() >= RUNNING;
}

bool ThreadWorker::IsRunning() const
{
	return state.load() == RUNNING;
}

bool ThreadWorker::IsTerminated() const
{
	return state.load() >= AWAITING_TERMINATE;
}

size_t ThreadWorker::WorkCount() const
{
	return size.load();
}

void ThreadWorker::Insert0(const Callable& func)
{
	std::unique_lock<std::recursive_mutex> writeLock(writeMutex);
	{
		if (state.load() > RUNNING)
		{
			throw "Terminated worker";
		}

		// Wait till space is available
		while (size.load() == capacity)
		{
			notFull.wait(writeLock);
		}

		Enqueue(func);

		size_t s = size++;
		if (s == 0)
		{
			// Another thread might be waiting
			// for not emtpy, so we signal them
			SignalNotEmpty();
		}
	}
}

bool ThreadWorker::TryInsert0(const Callable& func)
{
	std::unique_lock<std::recursive_mutex> writeLock(writeMutex);
	{
		// No wait here
		if (size.load() < capacity && state.load() < AWAITING_TERMINATE)
		{
			Enqueue(func);

			size_t s = size++;
			if (s == 0)
			{
				// Another thread might be waiting
				// for not emtpy, so we signal them
				SignalNotEmpty();
			}

			return true;
		}

		return false;
	}
}

void ThreadWorker::RunWorker()
{
	LOG << info() << "Initiating the worker loop";

	while (true)
	{
		Callable func;
		if (!TryTake(func)) // Currently no work to take
		{
			if (state.load() != AWAITING_TERMINATE) // We may wait for more work
			{
				if (timeoutMillis.count() > 0) // Timeout enabled
				{
					if (!Take(func)) // Couldn't take work before timeout, we should CAS back to PRE_START if RUNNING
					{
						LOG << debug() << "Timeout triggered";
						int s = RUNNING;
						if (state.compare_exchange_strong(s, PRE_START))
						{
							LOG << debug() << "CASed to PRE_START, exiting the worker loop";
						}
						else if(s == TERMINATED)
						{
							LOG << debug() << "TERMINATED state loaded, breaking the worker loop";
						}
						else
						{
							LOG << warn() << "Unexpected state on triggering timeout: " << s;
						}

						// Break
						break;
					}
				}
				else // Timeout disabled
				{
					func = Take();
				}
			}
			else // Don't wait for more work cuz we should terminate now
			{
				LOG << debug() << "AWAITING_TERMINATE state loaded, breaking the worker loop";

				// No work left and AWAITING_TERMINATE,
				// so we break and let AwaitAndTerminate
				// join us
				break;
			}
		}

		LOG << debug() << "Took work";

		if (state.load() == TERMINATED)
		{
			LOG << debug() << "TERMINATED state loaded, breaking the worker loop";
			// This is a TERMINATE_SIGNAL from TERMINATE, end the worker
			break;
		}

		try
		{
			LOG << debug() << "Executing popped task";

			func();
		}
		catch (...)
		{
			LOG << warn() << "An error occured while invoking a callable, continuing the worker loop";
		}
	}
}

ThreadWorker::Callable ThreadWorker::Take()
{
	LOG << info() << "Taking work...";

	Callable func;
	std::unique_lock<std::recursive_mutex> readLock(readMutex);
	{
		while (size.load() == 0)
		{
			notEmpty.wait(readLock);
		}

		func = Dequeue();

		size_t s = size--;
		if (s == capacity)
		{
			SignalNotFull();
		}
	}

	return func;
}

bool ThreadWorker::TryTake(Callable& funcRef)
{
	LOG << info() << "Taking work if available";

	std::unique_lock<std::recursive_mutex> readLock(readMutex);
	{
		if (size.load() > 0)
		{
			funcRef = Dequeue();

			size_t s = size--;
			if (s == capacity)
			{
				SignalNotFull();
			}

			return true;
		}

		return false;
	}
}

bool ThreadWorker::Take(Callable& funcRef)
{
	LOG << info() << "Taking work with timeout";

	std::unique_lock<std::recursive_mutex> readLock(readMutex);
	{
		LOG << debug() << "Waiting for: " << timeoutMillis.count();

		if (notEmpty.wait_for(readLock, timeoutMillis, [this]() { return size.load() != 0; }))
		{
			funcRef = Dequeue();

			size_t s = size--;
			if (s == capacity)
			{
				SignalNotFull();
			}

			return true;
		}

		return false;
	}
}

void ThreadWorker::Clear()
{
	LOG << info() << "Clearing the work queue";

	for (Node* next = head->next; next; next = head->next)
	{
		head->next = next->next; // Becums null ptr when tail is reached

		delete next;
	}

	tail = head;
	size.store(0);
}

// PRECONDITION -> dist(tail, head) < capacity && writeLock held by the curr thread
void ThreadWorker::Enqueue(const Callable& callable)
{
	tail->next = new Node(callable);
	tail = tail->next;
}

// PRECONDITION -> dist(tail, head) > 0 (at least one head->next write occurs) && readLock held by the curr thread
ThreadWorker::Callable ThreadWorker::Dequeue()
{
	// No data races here bec this method
	// won't be called unless a head->next write
	// occurs 
	Node* first = head->next;
	Callable func = std::move(first->func); // Move func
	Node* oldHead = head;
	head = first;

	delete oldHead;

	return func;
}

void ThreadWorker::SignalNotEmpty()
{
	std::unique_lock<std::recursive_mutex> readLock(readMutex);
	{
		notEmpty.notify_all();
	}
}

void ThreadWorker::SignalNotFull()
{
	std::unique_lock<std::recursive_mutex> writeLock(writeMutex);
	{
		notFull.notify_all();
	}
}

// Move min(remCapacity, worker.size) els from worker
void ThreadWorker::MoveFrom(ThreadWorker& worker)
{
	std::unique_lock<std::recursive_mutex> writeLock(writeMutex, std::defer_lock),
		insertLock(readMutex, std::defer_lock),
		_insertLock(worker.readMutex, std::defer_lock),
		_writeLock(worker.readMutex, std::defer_lock);
	std::lock(writeLock, insertLock, _insertLock, _writeLock); // if &worker == this it's ok cuz we r using recursive_mutexes
	{
		Node* newHead = new Node();
		Node* newTail = newHead;
		size_t newSize = 0;
		for (Node* next = worker.head->next; next && newSize < capacity; next = next->next)
		{
			newTail = next;
			newSize++;
		}

		if (newSize > 0) // Only change states if we took any elements
		{
			if (newTail->next) // More items are still in worker, set head's next to their first
			{
				worker.head->next = newTail->next;
			}
			else // Invalidate worker's tail
			{
				worker.head->next = nullptr;
				worker.tail = worker.head;
			}
			worker.size -= newSize;

			Clear(); // Delete old nodes, if any
			head = newHead;
			tail = newTail;
			size.store(newSize);

			// Move the thread along with the state
			thread = std::move(worker.thread);
			state.store(worker.state.load());
			worker.state.store(PRE_START); // Restart the other worker's state
		}
	}
}
