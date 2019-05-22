#pragma once

#include <functional>
#include <mutex>
#include <atomic>
#include <thread>
#include <condition_variable>

#include "Helper/Logger/SimpleLogger.h"
#include "Core/NoCopy.h"

class ThreadWorker :
	NoCopy
{
public:
	using Callable = std::function<void(void)>;
public:
	ThreadWorker(uint32_t capacity, std::chrono::milliseconds timeoutMillis = std::chrono::milliseconds(0LL));
	~ThreadWorker();

	ThreadWorker(ThreadWorker&& other) noexcept;
	ThreadWorker& operator=(ThreadWorker&& other) noexcept;

	template<typename Func, typename... Args>
	void Submit(Func&& func, Args&& ... args)
	{
		Insert0(std::bind(std::forward<Func>(func), std::forward<Args>(args)...));
	}

	template<typename Func, typename... Args>
	bool TrySubmit(Func&& func, Args&& ... args)
	{
		return TryInsert0(std::bind(std::forward<Func>(func), std::forward<Args>(args)...));
	}

	bool Start();

	void TerminateNow();

	void AwaitTermination();

	bool IsStarted() const;

	bool IsRunning() const;

	bool IsTerminated() const;

	size_t WorkCount() const;
private:
	class Node
	{
		friend class ThreadWorker;
	private:
		Node();
		Node(const Callable& func);
		~Node() = default;

		Node* next;
		Callable func;
	};

	void Insert0(const Callable& func);

	bool TryInsert0(const Callable& func);

	void RunWorker();

	Callable Take();

	bool TryTake(Callable& funcRef);

	bool Take(Callable& funcRef);

	// PRECONDITION -> Both locks must be held by the calling thread
	void Clear();

	// PRECONDITION -> dist(tail, head) < capacity && writeLock held by the curr thread
	void Enqueue(const Callable& callable);

	// PRECONDITION -> dist(tail, head) > 0 && readLock held by the curr thread
	Callable Dequeue();

	void SignalNotEmpty();

	void SignalNotFull();

	void MoveFrom(ThreadWorker& worker);
private:
	// ThreadWorker stuff
	static const int PRE_START;
	static const int RUNNING;
	static const int AWAITING_TERMINATE;
	static const int TERMINATED;
	static const Callable TERMINATED_SIGNAL;
	static const SimpleLogger LOG;

	std::thread thread;
	std::atomic<int> state;

	// Queue stuff
	Node* head;
	Node* tail;
	uint32_t capacity;
	std::atomic<size_t> size;
	std::chrono::milliseconds timeoutMillis;
	std::recursive_mutex readMutex, writeMutex;
	std::condition_variable_any notEmpty, notFull;
};
