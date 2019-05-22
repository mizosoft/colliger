#pragma once

#include "Direction.h"
#include "Helper/Event/WindowEventListener.h"
#include "Helper/Event/WindowEventDispatcher.h"

class MovementListener :
	public WindowEventListener
{
public:
	MovementListener();
	~MovementListener() = default;
public:
	virtual void Handle(sf::Event& event, Game& context) override;

	Direction GetCurrentDirection();
private:
	Direction direction;
};
