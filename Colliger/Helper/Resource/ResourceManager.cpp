#include "ResourceManager.h"

ResourceManager::LoadTaskScopedState::LoadTaskScopedState(ResourceManager::LoadTask& theTask) :
	theTask(theTask)
{
	int s = LoadTask::PRE_START;
	execute = theTask.state.compare_exchange_strong(s, LoadTask::STARTED);
}

ResourceManager::LoadTaskScopedState::~LoadTaskScopedState()

{
	int s = LoadTask::STARTED;
	if (theTask.state.compare_exchange_strong(s, LoadTask::FINISHED))
	{
		theTask.loadCounter++;
	}
}
bool ResourceManager::LoadTaskScopedState::ShouldExecute()
{
	return execute;
}
//////////////////////////////////////////////////////////////////////////////////////////////////

const int ResourceManager::LoadTask::PRE_START = 1;
const int ResourceManager::LoadTask::STARTED = 2;
const int ResourceManager::LoadTask::FINISHED = 3;

ResourceManager::LoadTask::LoadTask(LoadFunc loadFunc, const std::string& fileName, std::atomic<int>& loadCounter) :
	loadFunc(std::move(loadFunc)),
	loadCounter(loadCounter),
	state(PRE_START),
	fileName(fileName)
{
	promisePtr = std::make_shared<std::promise<Resource>>();
}

ResourceManager::LoadTask::LoadTask(LoadTask&& other) noexcept :
	loadCounter(other.loadCounter)
{
	MoveFrom(other);
}

ResourceManager::LoadTask& ResourceManager::LoadTask::operator=(LoadTask&& other) noexcept
{
	MoveFrom(other);

	return *this;
}

ResourceManager::LoadTask::LoadTask(const LoadTask& other) :
	loadCounter(other.loadCounter)
{
	CopyFrom(other);
}

ResourceManager::LoadTask& ResourceManager::LoadTask::operator=(const LoadTask& other)
{
	CopyFrom(other);

	return *this;
}

void ResourceManager::LoadTask::operator()()
{
	LoadTaskScopedState state(*this);
	if (state.ShouldExecute())
	{
		try
		{
			promisePtr->set_value(loadFunc(fileName));
		}
		catch (...)
		{
			promisePtr->set_exception(std::current_exception());
		}
	}
}

ResourceManager::ResourceFuture ResourceManager::LoadTask::GetFuture()
{
	return promisePtr->get_future();
}

void ResourceManager::LoadTask::MoveFrom(LoadTask& other)
{
	int s;
	while (!other.state.compare_exchange_strong(s, FINISHED));
	state.store(s);
	fileName = std::move(other.fileName);
	promisePtr = std::move(other.promisePtr);
	loadFunc = std::move(other.loadFunc);
}

void ResourceManager::LoadTask::CopyFrom(const LoadTask& other)
{
	state.store(other.state.load());
	fileName = other.fileName;
	promisePtr = other.promisePtr;
	loadFunc = other.loadFunc;
}

//////////////////////////////////////////////////////////////////////////////////////////////////

const int ResourceManager::FULLY_LOADED = 100;
const int ResourceManager::WORKER_TIMEOUT_MILLIS = 10 * 1000; // 10 seconds;

ResourceManager::ResourceManager(int tasksPerThread) :
	loadCounter(0),
	mutex(),
	waiting(),
	resources(),
	resolver(std::make_unique<PathResolver>()),
	worker(tasksPerThread, std::chrono::milliseconds(WORKER_TIMEOUT_MILLIS))
{
}

ResourceManager::~ResourceManager()
{
	std::unique_lock<std::mutex> lock(mutex);
	{
		while (waiting.size() > 0) waiting.pop();
		resources.clear();
	}

	worker.TerminateNow();
	loadCounter.store(0);
}

int ResourceManager::UpdateLoading()
{
	{
		std::unique_lock<std::mutex> lock(mutex);
		{
			while (waiting.size() > 0)
			{
				if (worker.TrySubmit(waiting.front()))
				{
					waiting.pop();
				}
				else
				{
					break;
				}
			}
		}
	}

	if (!worker.IsStarted())
	{
		worker.Start();
	}

	return static_cast<int>(std::round(((size_t) FULLY_LOADED * loadCounter.load()) / resources.size()));
}

void ResourceManager::SetPathResolver(PathResolver* resolver)
{
	std::unique_lock<std::mutex> lock(mutex);
	{
		if (resolver)
		{
			this->resolver.reset(resolver);
		}
	}
}
