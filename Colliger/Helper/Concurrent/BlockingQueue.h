#pragma once

/*
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <memory>

template<class T>
class BlockingQueue;

// A simple linked-list node class
template<class T>
struct Node
{
	friend class BlockingQueue<T>;
private:
	Node() :
		next(nullptr)
	{ 
	}
	Node(std::unique_ptr<T>&& item) :
		item(std::move(item)),
		next(nullptr)
	{
	}
	~Node() = default;
private:
	std::unique_ptr<T> item;
	Node<T>* next;
};

// A thread-safe queue that blocks waiting for
// elements, it is very similar structurally
// to java's LinkedBlockingQueue written 
// by Doug Lee 
template<typename T>
class BlockingQueue
{
public:
	BlockingQueue(uint32_t capacity) :
		capacity(capacity),
		size(0)
	{
		head = tail = new Node<T>();
	}
	~BlockingQueue()
	{
		Clear();
	}

	BlockingQueue(const BlockingQueue& other) :
		capacity(other.capacity)
	{
		CopyFrom(other);

		return *this;
	}

	BlockingQueue& operator=(const BlockingQueue& other)
	{
		CopyFrom(other);
	}

	BlockingQueue(BlockingQueue<T>&& other) :
		capacity(other.capacity)
	{
		MoveFrom(other);
	}

	BlockingQueue& operator=(BlockingQueue&& other)
	{
		MoveFrom(other);
	}

	void Insert(const T& val)
	{
		Insert0(std::make_unique<T>(val));
	}

	void Insert(T&& val)
	{
		Insert0(std::make_unique<T>(std::move(val)));
	}

	bool TryInsert(const T& val)
	{
		return TryInsert0(std::make_unique<T>(val));
	}

	bool TryInsert(T&& val)
	{
		return TryInsert0(std::make_unique<T>(std::move(val)));
	}

	std::unique_ptr<T> Take()
	{
		std::unique_ptr<T> ptr;
		std::unique_lock<std::recursive_mutex> takeLock(takeMutex);
		{
			while (size.load() == 0)
			{
				notEmpty.wait(takeLock);
			}
		
			ptr = Dequeue();

			int s = size--;
			if (s == capacity) // Was previously full, notify possibly blocking threads awaiting notFull
			{
				std::unique_lock<std::recursive_mutex> intertLock(insertMutex);
				{
					notFull.notify_all();
				}
			}
		}

		return ptr;
	}

	void Clear()
	{
		std::unique_lock<std::recursive_mutex> takeLock(takeMutex), insertLock(insertMutex);
		{
			Clear0();
		}
	}

	int GetCapacity()
	{
		return capacity;
	}
private:
	void Insert0(std::unique_ptr<T>&& ptr)
	{
		Node<T>* node = new Node<T>(std::move(ptr));
		std::unique_lock<std::recursive_mutex> insertLock(insertMutex);
		{
			while (size.load() == capacity)
			{
				notFull.wait(insertLock);
			}

			Enqueue(node);

			int s = size++;
			if (s == 0) // Was previously empty, notify possibly blocking threads awaiting notEmpty
			{
				std::unique_lock<std::recursive_mutex> read(takeMutex);
				{
					notEmpty.notify_all();
				}
			}
		}
	}

	bool TryInsert0(std::unique_ptr<T>&& ptr)
	{
		std::unique_lock<std::recursive_mutex> insertLock(insertMutex);
		{
			if (size.load() < capacity)
			{
				Enqueue(new Node<T>(std::move(ptr)));

				return true;
			}

			return false;
		}
	}

	void Enqueue(Node<T>* node)
	{
		tail->next = node;
		tail = tail->next;
	}

	std::unique_ptr<T> Dequeue()
	{
		// No races here because Dequeue is never
		// called unless the a head->next write
		// happens
		Node<T>* f = head->next;
		std::unique_ptr<T> item = std::move(f->item); // Move ownership, now f is empty 
		Node<T>* h = head;
		// Make the empty f node head, instead of setting 
		// head->next = f->next which will cause races if
		// another thread is inserting at f->next if f is the tail
		head = f; 

		delete h;

		return item;
	}

	// Lock MUST be held before calling this method
	void Clear0()
	{
		Node<T>* next = nullptr;
		while (next = head->next)
		{
			head->next = next->next;

			delete next;
		}

		// head->item.reset(nullptr); Not needed ?
		tail = head;
		size.store(0);
	}

	void CopyFrom(BlockingQueue<T>& right)
	{
		std::unique_lock<std::recursive_mutex> otherInsertLock(right.insertMutex, std::defer_lock), 
			otherTakeLock(right.takeMutex, std::defer_lock);
		std::unique_lock<std::recursive_mutex> insertLock(insertMutex, std::defer_lock), 
			takeLock(takeMutex, std::defer_lock);
		std::lock(otherInsertLock, otherTakeLock, takeLock, insertLock);
		{
			Clear0(); // Sets head = head = new Node<T>();
			Node<T> *otherHead = right.head;
			for (Node<T> next = otherHead->next; next; next = otherHead->next)
			{
				Enqueue(std::make_unique<T>(*(next->item)));
			}
		}
	}

	// Moves min(capacity, right.size) from right
	void MoveFrom(BlockingQueue<T>& right)
	{
		Node<T>* newHead = new Node<T>();
		Node<T>* newTail = newHead;
		int newSize = 0;
		// right lock scop
		{	
			std::unique_lock<std::recursive_mutex> otherInsertLock(right.insertMutex, std::defer_lock),
				otherTakeLock(right.takeMutex, std::defer_lock);
			std::lock(otherInsertLock, otherTakeLock);
			{
				for (Node<T>* otherNext = right.head->next; otherNext && newSize <= capacity; otherNext = otherNext->next)
				{
					if (!newHead->next) // Initial iteration
					{
						newHead->next = otherNext;
					}

					newTail = otherNext;
					newSize++;
				}

				if (size > 0)
				{
					if (newTail->next) // There are elements still in right
					{
						right.head = new Node<T>();
						right.head->next = newTail->next;
						newTail->next = nullptr; // Unlink
					}
					else
					{
						right.tail = right.head = new Node<T>();
					}
				}
			}
		}
		

		std::unique_lock<std::recursive_mutex> insertLock(right.insertMutex, std::defer_lock), 
			takeLock(right.takeMutex, std::defer_lock);
		std::lock(insertLock, takeLock);
		{
			Clear0();
			head = newHead;
			tail = newTail;
		}
	}
private:
	Node<T>* head;
	Node<T>* tail;
	const uint32_t capacity;
	std::atomic<uint32_t> size;
	std::condition_variable notEmpty, notFull;
protected:
	std::recursive_mutex insertMutex, takeMutex;
};
*/