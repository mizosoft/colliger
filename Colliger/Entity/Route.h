#pragma once

#include <list>

#include "Direction.h"
#include "Tile/Tile.h"
#include "Helper/Shape/DrawablePath.h"

class MoveableEntity;

class Route  :
	public Drawable
{
public:
	Route(const std::list<Tile>& tileRoute);
	~Route() = default;

	bool HasNext();

	Direction GetNext(const MoveableEntity& entity);

	virtual void Draw(sf::RenderTarget& target, sf::RenderStates states) const override;
private:
	std::list<Tile> tileRoute;
};
