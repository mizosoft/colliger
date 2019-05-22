#include "Line.h"
#include "Helper/Utils/Math.h"

/*
 p2
  \
   \
    \
	|\
	| p1
	\ |  < First set the
	 \|  < rectangle like
	  |  < this then apply
	  |  < the correct  
	  |  < rotation
	  .
*/

// SFML doesn't support lines so normal rectangles 
// are used but this is tricky because the correct 
// rotation should be applied after setting one 
// vertex to an arbitrary point

const float Line::DEFAULT_THICKNESS = 1.f;

Line::Line(const sf::Vector2f& p1, const sf::Vector2f& p2) :
	line()
{
	Reset(p1, p2);
	SetThickness(DEFAULT_THICKNESS);
}

/*Line::Line(sf::Vector2f p1, sf::Vector2f p2, float thickness, sf::Color color) :
	line()
{
	line.setPosition(p1);
	line.setOrigin({ thickness / 2.f, 0.f });

	sf::Vector2f v1(0.f, line.getSize().y), v2(p2.x - p1.x, p2.y - p1.y);
	float angleDegrees = GetAngleDegrees(v1, v2);
	line.setRotation(p2.x < p1.x ? angleDegrees : -angleDegrees);

	line.setFillColor(color);
	Reset(p1, p2);
	SetThickness(thickness);
	SetFillColor(color);
}*/

void Line::SetThickness(float thickness)
{
	line.setSize({ thickness, line.getSize().y });
	line.setOrigin({ DEFAULT_THICKNESS / 2.f, 0.f });
}

void Line::SetFillColor(const sf::Color& color)
{
	line.setFillColor(color);
}

void Line::Reset(const sf::Vector2f& p1, const sf::Vector2f& p2)
{
	float thickness = line.getSize().x;
	line.setSize({ thickness, GetDistance(p1, p2) });
	line.setOrigin({ thickness / 2.f, 0.f });
	line.setPosition(p1);

	sf::Vector2f v1(0.f, line.getSize().y), v2(p2.x - p1.x, p2.y - p1.y);
	float angleDegrees = GetAngleDegrees(v1, v2);
	line.setRotation(p2.x < p1.x ? angleDegrees : -angleDegrees);
}

sf::Vector2f Line::GetBeginning() const
{
	return line.getPosition();
}

void Line::Draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(line, states);
}
