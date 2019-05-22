#include "MovementListener.h"

MovementListener::MovementListener() : 
	direction(NO_MOVEMENT)
{
}

void MovementListener::Handle(sf::Event& event, Game& context)
{
	switch (event.key.code)
	{
	case sf::Keyboard::Left:
		direction = LEFT;

		break;
	case sf::Keyboard::Right:
		direction = RIGHT;

		break;
	case sf::Keyboard::Up:
		direction = UP;

		break;
	case sf::Keyboard::Down:
		direction = DOWN;

		break;
	}
}

Direction MovementListener::GetCurrentDirection()
{
	return direction;
}
