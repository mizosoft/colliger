#pragma once

#include <set>
#include <map>
#include <list>

#include "Core/Drawable.h"
#include "Helper/Shape/Line.h"
#include "Helper/Shape/Point.h"
#include "Helper/Logger/SimpleLogger.h"
#include "Tile.h"

class TileMap;

class TileGraph : 
	public Drawable
{
public:
	TileGraph(const TileMap& tileMap);
	~TileGraph() = default;

	void InitializeGraph();

	void InitializeDebugModeObjects();

	bool IsInitialized();

	const TileMap& GetTileMap() const;

	void FindShortestPath(const sf::Vector2f& pos1, const sf::Vector2f& pos2,  std::list<Tile>& list) const;
private:
	void DecriptIncremental(int index, sf::Vector2i& posInMap) const;

	int EncriptIncremental(int i, int j) const;

	int NormalizeIncremental(int query) const;

	void BuildPath(int src, int dest, std::list<Tile>& list) const;

	virtual void Draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	// bool IsAdjacent(const Tile& tile1, const Tile& tile2) const;
private:
	static const SimpleLogger LOG;
	static const float DEBUG_NODE_POINT_THICKNESS;
	static const float DEBUG_EDGE_LINE_THICKNESS;

	bool initialized;
	const TileMap& tileMap;
	std::map<int, std::set<int>> adjacencyMap;
	// map<sourceNode, map<destNode, pair<viaNode, score>>>
	mutable std::map<int, std::map<int, std::pair<int, int>>> shortestPathCache;

	// debug objs
	std::list<Point> debugModeNodes;
	std::list<Line> debugModeEdges;
};
