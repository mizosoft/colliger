#pragma once

#include <map>
#include <memory>
#include <list>

#include"StatelessEventListener.h"

template<typename ID, typename... Args>
class EventDispatcher
{
private:
	using Listener = EventListener<Args...>;
	using Callback = typename StatelessEventListener<Args...>::Callback;
public:
	EventDispatcher() = default;
	~EventDispatcher() = default;

	template<typename _Listener>
	std::shared_ptr<_Listener> Subscribe(ID id, const _Listener& listener)
	{
		auto listenerPtr = std::make_shared<_Listener>(listener);
		listeners[id].push_back(std::static_pointer_cast<Listener>(listenerPtr)); // Converted to weak

		return listenerPtr;
	}

	template<typename _Listener>
	std::shared_ptr<_Listener> Subscribe(ID id, _Listener&& listener)
	{
		auto listenerPtr = std::make_shared<_Listener>(std::move(listener));
		listeners[id].push_back(std::static_pointer_cast<Listener>(listenerPtr)); // Converted to weak

		return listenerPtr;
	}

	std::shared_ptr<Listener> SubscribeCallback(ID id, const Callback& callback)
	{
		return Subscribe(id, StatelessEventListener<Args...>(callback));
	}

	std::shared_ptr<Listener> SubscribeCallback(ID id, Callback&& callback)
	{
		return Subscribe(id, StatelessEventListener<Args...>(std::move(callback)));
	}

#include <iostream>

	void Fire(ID id, Args... args)
	{
		Clean();

		auto iter = listeners.find(id);
		if (iter != listeners.end())
		{
			for (auto& listenerPtrWeak : iter->second)
			{
				auto listenerPtr = listenerPtrWeak.lock();
				if (listenerPtr)
				{
					listenerPtr->Handle(args...);
				}
			}
		}
	}
private:
	void Clean()
	{
		std::list<ID> toRemove;
		for (auto& entry : listeners)
		{
			auto& list = entry.second;
			list.remove_if(std::bind(&std::weak_ptr<Listener>::expired, std::placeholders::_1));
			if (list.empty())
			{
				toRemove.push_back(entry.first);
			}
		}

		for (const ID& id : toRemove)
		{
			listeners.erase(id);
		}
	}
private:
	std::map<ID, std::list<std::weak_ptr<Listener>>> listeners;
};

/*template<typename... Args>
class EventDispatcher<void, Args...>
{
	using Listener = EventListener<Args...>;
	using Callback = typename StatelessEventListener<Args...>::Callback;
public:
	EventDispatcher() = default;
	~EventDispatcher() = default;

	template<typename _Listener>
	std::shared_ptr<_Listener> Subscribe(const _Listener& listener)
	{
		auto listenerPtr = std::make_shared<_Listener>(listener);
		listeners.push_back(); // Converted to weak

		return listenerPtr;
	}

	template<typename _Listener>
	std::shared_ptr<_Listener> Subscribe(_Listener&& listener)
	{
		auto listenerPtr = std::make_shared<_Listener>(std::move(listener));
		listeners.push_back(std::static_pointer_cast<Listener>(listenerPtr)); // Converted to weak

		return listenerPtr;
	}

	std::shared_ptr<Listener> SubscribeCallback(const Callback& callback)
	{
		return Subscribe(StatelessEventListener<Args...>(callback));
	}

	std::shared_ptr<Listener> SubscribeCallback(Callback&& callback)
	{
		return Subscribe(StatelessEventListener<Args...>(std::move(callback)));
	}

	void Fire(Args... args)
	{
		Clean();

		for (auto& listenerPtrWeak : listeners)
		{
			std::shared_ptr<Listener> listenerPtr = listenerPtrWeak.lock();
			if (listenerPtr)
			{
				listenerPtr->Handle(args...);
			}
		}
	}
private:
	void Clean()
	{
		listeners.remove_if(std::bind(&std::weak_ptr<Listener>::expired, std::placeholders::_1));
	}
private:
	std::list<std::weak_ptr<Listener>> listeners;
};
*/