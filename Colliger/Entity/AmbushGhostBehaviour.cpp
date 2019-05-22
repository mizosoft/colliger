#include "AmbushGhostBehaviour.h"

#include "Ghost.h"
#include "Tile/TileGraph.h"
#include "Colliger.h"
#include "World/GameWorld.h"

Route AmbushGhostBehaviour::GetNextRoute(const Ghost& ghost)
{
	const GameWorld& gameWorld = ghost.GetGameWorld();
	const TileGraph& tileGraph = gameWorld.GetTileGraph();
	const Colliger& player = gameWorld.GetPlayer();
	std::list<Tile> path;
	tileGraph.FindShortestPath(ghost.GetCenterPosition(), player.GetCenterPosition(), path);

	return Route(path);
}
