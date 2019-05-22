#pragma once

#include <list>

#include "Helper/Shape/Line.h"
#include "Core/Drawable.h"

class DrawablePath :
	public Drawable
{
public:
	DrawablePath(std::list<sf::Vector2f> pnts, float thickness, sf::Color color);
	~DrawablePath() = default;

	virtual void Draw(sf::RenderTarget& target, sf::RenderStates states) const override;
private:
	std::list<Line> lines;
};

