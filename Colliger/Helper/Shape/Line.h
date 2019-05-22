#pragma once

#include "SFML/Graphics/RectangleShape.hpp"
#include "Core/Drawable.h"

class Line : 
	public Drawable
{
public:
	Line(const sf::Vector2f& p1, const sf::Vector2f& p2);
	~Line() = default;

	void SetThickness(float thickness);

	void SetFillColor(const sf::Color& color);

	void Reset(const sf::Vector2f& p1, const sf::Vector2f& p2);

	sf::Vector2f GetBeginning() const;
private:
	virtual void Draw(sf::RenderTarget& target, sf::RenderStates states) const override;
private:
	static const float DEFAULT_THICKNESS;

	sf::RectangleShape line;
};
