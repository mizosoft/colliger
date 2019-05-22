#pragma once

template<typename... Args>
class EventListener
{
public:
	virtual ~EventListener() = default;

	virtual void Handle(Args... args) { }
};
