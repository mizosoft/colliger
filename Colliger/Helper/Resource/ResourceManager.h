#pragma once

#include <map>
#include <future>
#include <queue>
#include <typeindex>

#include "Core/NoCopy.h"
#include "Loader.h"
#include "PathResolver.h"
#include "Helper/Concurrent/ThreadWorker.h"
#include "CaseInsensitiveOrder.h"

#if defined(_WIN32)
#define _FILE_NAME_COMP CaseInsensitiveOrder
#else
#define _FILE_NAME_COMP std::less<std::string>
#endif

class ResourceManager :
	NoCopy
{
	friend class LoadTask;
	friend class LoadTaskScopedState;
private:
	using Resource = std::shared_ptr<void>;
	using ResourceFuture = std::shared_future<Resource>;
	using ResourceEntry = std::pair<std::type_index, ResourceFuture>;
	// using ResourceFuturePtr = std::shared_ptr<std::shared_future<Resource>>;
	using ResourceMap = std::map<std::string, ResourceEntry, _FILE_NAME_COMP>;
public:
	ResourceManager(int tasksPerThread);
	~ResourceManager();

	template<typename T>
	void Enqueue(const std::string& fileName)
	{
		std::unique_lock<std::mutex> lock(mutex);
		{
			std::type_index type = typeid(T);
			ResourceMap::iterator iter = resources.find(fileName);
			if (iter != resources.end())
			{
				if (iter->second.first != type)
				{
					throw std::invalid_argument("Resource[" + fileName + "] is enqueued with type[" +
						iter->second.first.name() + "] but requested to be enqueued again with type[" + type.name() + "]");
				}
			}
			else // Only add a task once
			{
				Loader<T> loader;
				LoadTask::LoadFunc loadFunc = std::bind([this](Loader<T> _loader, const std::string & _fileName)
					{
						return std::static_pointer_cast<void>(_loader(_fileName, resolver));
					}, loader, std::placeholders::_1);
				LoadTask task(loadFunc, resolver->Resolve(fileName), loadCounter);
				resources.emplace(fileName, std::make_pair(type, task.GetFuture()));
				waiting.push(std::move(task));
			}
		}
	}

	int UpdateLoading();

	template<typename T>
	std::shared_ptr<T> GetResource(const std::string & fileName)
	{
		std::shared_ptr<T> ptr;
		std::unique_lock<std::mutex> lock(mutex);
		{
			ResourceMap::iterator iter = resources.find(fileName);

			if (iter == resources.end())
			{
				throw std::invalid_argument("Resource[" + fileName + "] hasn't been enqueued");
			}
			else
			{
				std::type_index type = typeid(T);
				if (iter->second.first != type)
				{
					throw std::invalid_argument("Resources[" + fileName + "] with type is enqueued with type[" +
						iter->second.first.name() + "] but requested with type[" + type.name() + "]");
				}

				return std::static_pointer_cast<T>(iter->second.second.get());;
			}
		}
	}

	void SetPathResolver(PathResolver* resolver);
private:
	class LoadTask;

	class LoadTaskScopedState
	{
	public:
		LoadTaskScopedState(LoadTask& theTask);
		~LoadTaskScopedState();

		bool ShouldExecute();
	private:
		bool execute;
		LoadTask& theTask;
	};

	class LoadTask :
		NoCopy
	{
	public:
		using LoadFunc = std::function<std::shared_ptr<void>(const std::string& fileName)>;
	public:
		LoadTask(LoadFunc loadFunc, const std::string& fileName, std::atomic<int>& loadCounter);
		~LoadTask() = default;

		LoadTask(LoadTask&& other) noexcept;
		LoadTask& operator=(LoadTask&& other) noexcept;

		LoadTask(const LoadTask& other);
		LoadTask& operator=(const LoadTask& other);

		void operator()();

		ResourceFuture GetFuture();
	private:
		void MoveFrom(LoadTask& other);

		void CopyFrom(const LoadTask& other);
	public:
		static const int PRE_START;
		static const int STARTED;
		static const int FINISHED;

		std::atomic<int>& loadCounter;
		std::atomic<int> state;
		std::string fileName;
		std::shared_ptr<std::promise<Resource>> promisePtr;
		LoadFunc loadFunc;
	};
public:
	static const int FULLY_LOADED;
	static const int WORKER_TIMEOUT_MILLIS;
private:
	std::atomic<int> loadCounter;
	std::mutex mutex;
	std::queue<LoadTask> waiting;
	ResourceMap resources;
	std::shared_ptr<PathResolver> resolver;
	ThreadWorker worker;
};
