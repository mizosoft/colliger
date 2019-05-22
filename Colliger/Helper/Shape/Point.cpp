#include "Point.h"

const float Point::DEFAULT_THICKNESS = 2.f;

Point::Point(const sf::Vector2f& center) :
	point(DEFAULT_THICKNESS / 2.f)
{
	point.setOrigin(point.getRadius(), point.getRadius());
	point.setPosition(center);
}

/*Point::Point(sf::Vector2f pos, float thickness, sf::Color color) :
	point(thickness / 2.f)
{
	point.setOrigin(point.getRadius(), point.getRadius());
	point.setPosition(pos);
	
	point.setFillColor(color);
}*/

void Point::SetCenter(const sf::Vector2f& center)
{
	point.setOrigin(point.getRadius(), point.getRadius());
	point.setPosition(center);
}

sf::Vector2f Point::GetCenter()
{
	return point.getPosition();
}

void Point::SetThickness(float thickness)
{
	point.setRadius(thickness / 2.f);
	point.setOrigin(point.getRadius(), point.getRadius());
}

void Point::SetFillColor(const sf::Color& color)
{
	point.setFillColor(color);
}

void Point::Draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(point, states);
}
