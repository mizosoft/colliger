#pragma once

#include "SFML/Graphics/View.hpp"

class Viewport
{
public:
	Viewport(const sf::FloatRect& worldView = sf::FloatRect());
	virtual ~Viewport() = default;

	sf::View& GetView();

	virtual void Update(const sf::Vector2u& newWindowSize) = 0;
protected:
	sf::View view;
};
