#pragma once

#include "SFML/Graphics/CircleShape.hpp"
#include "Core/Drawable.h"

class Point :
	public Drawable
{
public:
	Point(const sf::Vector2f& center = sf::Vector2f());
	~Point() = default;

	void SetCenter(const sf::Vector2f& center);

	sf::Vector2f GetCenter();

	void SetThickness(float thickness);

	void SetFillColor(const sf::Color& color);
private:
	virtual void Draw(sf::RenderTarget& target, sf::RenderStates states) const override;
private:
	static const float DEFAULT_THICKNESS;

	sf::CircleShape point;
};