#include "Route.h"

#include "Entity/MoveableEntity.h"
#include "Tile/TileMap.h"
#include "World/GameWorld.h"

Route::Route(const std::list<Tile>& tileRoute) :
	tileRoute(tileRoute)
{
}

bool Route::HasNext()
{
	return !tileRoute.empty();
}

Direction Route::GetNext(const MoveableEntity& entity)
{
	const Tile& nextTile = tileRoute.front();
	const Tile& entityTile = entity.GetEntityTile();
	const TileMap& tileMap = entity.GetGameWorld().GetTileMap();
	int i = nextTile.GetRowIndex(), j = nextTile.GetColumnIndex();
	int _i = entityTile.GetRowIndex(), _j = entityTile.GetColumnIndex();
	std::cout << std::endl << i << ", " << j << ", " << _i << ", " << _j << std::endl;
	if (i > _i)
	{
		return Direction::DOWN;
	}
	else if (i < _i)
	{
		return Direction::UP;
	}
	else if (j > _j)
	{
		return Direction::RIGHT;
	}
	else if (j < _j)
	{
		return Direction::LEFT;
	}
	else
	{
		tileRoute.pop_front();

		return entity.GetDirection();
	}
}

void Route::Draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	std::list<sf::Vector2f> points;
	for (const Tile& tile : tileRoute)
	{
		points.push_back(tile.GetTileCenter());
	}

	DrawablePath path(points, 5, sf::Color::Cyan);
	target.draw(path, states);
}
