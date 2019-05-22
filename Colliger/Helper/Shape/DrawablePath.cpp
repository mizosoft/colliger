#include "DrawablePath.h"

DrawablePath::DrawablePath(std::list<sf::Vector2f> pnts, float thickness, sf::Color color)
{
	if (pnts.size() > 0)
	{
		sf::Vector2f initial = pnts.back();
		pnts.pop_back();
		while (!pnts.empty())
		{
			sf::Vector2f pnt = pnts.back();
			pnts.pop_back();
			lines.emplace_back(initial, pnt);
			lines.rbegin()->SetThickness(thickness);
			lines.rbegin()->SetFillColor(color);
			initial = pnt;
		}
	}
}

void DrawablePath::Draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (const Line& line : lines)
	{
		target.draw(line, states);
	}
}
