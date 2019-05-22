#pragma once

#include <functional>

#include "EventListener.h"

template<typename... Args>
class StatelessEventListener :
	public EventListener<Args...>
{
public:
	using Callback = std::function<void(Args...)>;
public:
	StatelessEventListener(const Callback& callback) :
		callback(callback)
	{
	}
	~StatelessEventListener() = default;

	virtual void Handle(Args... args) override
	{
		callback(args...);
	}
private:
	Callback callback;
};
