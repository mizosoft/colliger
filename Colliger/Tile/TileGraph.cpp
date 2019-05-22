#include "TileGraph.h"

#include <queue>
#include <set>

#include "TileMap.h"

const SimpleLogger TileGraph::LOG = SimpleLogger("TileGraph");
const float TileGraph::DEBUG_NODE_POINT_THICKNESS = 4.0;
const float TileGraph::DEBUG_EDGE_LINE_THICKNESS = 3.0;

const auto min = [](const int& i1, const int& i2) { return i1 <= i2 ? i1 : i2; };

TileGraph::TileGraph(const TileMap& tileMap) :
	initialized(false),
	tileMap(tileMap)
{
}

void TileGraph::InitializeGraph()
{
	LOG << info() << "Initializing the TileGraph";

	// If called more than once
	adjacencyMap.clear();

	int mapWidth = tileMap.GetMapWidth(),
		mapHeight = tileMap.GetMapHeight();

	int rows = mapHeight, columns = mapWidth;
	// An (EMPTY (true) or non EMPTY (false)) Tile representation of the map
	bool** tiles = new bool* [rows];
	for (int i = 0; i < rows; i++)
	{
		tiles[i] = new bool[columns] { false };
	}
	for (const TileLayer& layer : tileMap.GetLayers())
	{
		for (const std::vector<Tile> row : layer.GetTiles())
		{
			for (const Tile& tile : row)
			{
				tiles[tile.GetRowIndex()][tile.GetColumnIndex()] = tile.GetToken() == Tile::Token::EMPTY;
			}
		}
	}

	// Initialize the adjacencyMap
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < columns; j++)
		{
			// Only add the incremental index if the tile is empty at [i][j]
			if (tiles[i][j])
			{
				int index = EncriptIncremental(i, j); // The incremental index
				adjacencyMap[index] = std::set<int>(); // Initialize empty adjacencies

				LOG << debug() << "Inserting node of incremental index " << index
					<< ", of map index(" << i << ", " << j << ")";

				// A tile graph have adjacencies along the square surrounding it
				for (int _i = i - 1; _i <= i + 1; _i++)
				{
					for (int _j = j - 1; _j <= j + 1; _j++)
					{
						if (_i >= 0 &&
							_i < rows &&
							_j >= 0 &&
							_j < columns &&
							// Unlike a normal graph, there can't be
							// any diagonal edges (one index should be
							// equal), also adding the node itself again
							// should be avoided, which all result in a
							// XOR exp of index equality
							((_i == i) ^ (_j == j)) &&
							tiles[_i][_j])
						{
							int _index = EncriptIncremental(_i, _j);

							LOG << debug() << "Inserting node[" << index << "]'s adjacency of incremental index " << _index
								<< ", of map index(" << _i << ", " << _j << ")";

							// Insert the incremental index of the adjacency
							adjacencyMap[index].insert(_index);
						}
					}
				}
			}
		}
	}

	delete[] tiles;

	initialized = true;
}

void TileGraph::InitializeDebugModeObjects()
{
	LOG << info() << "Initializing debug mode objects";

	// If called more than once
	debugModeNodes.clear();
	debugModeEdges.clear();

	int mapWidth = tileMap.GetMapWidth(),
		mapHeight = tileMap.GetMapHeight(),
		tileWidth = tileMap.GetTileWidth(),
		tileHeight = tileMap.GetTileHeight();

	for (const std::pair<int, std::set<int>>& pair : adjacencyMap)
	{
		// Decript the incremental index for convenience
		sf::Vector2i posInMap;
		DecriptIncremental(pair.first, posInMap);
		sf::Vector2f nodePos(tileWidth / 2.f + tileWidth * posInMap.y,
			tileHeight / 2.f + tileHeight * posInMap.x);
		debugModeNodes.emplace_back(nodePos);
		debugModeNodes.rbegin()->SetThickness(DEBUG_NODE_POINT_THICKNESS);
		debugModeNodes.rbegin()->SetFillColor(sf::Color::Green);

		for (int adjacent : pair.second)
		{
			// Decript the incremental index for convenience
			sf::Vector2i _posInMap;
			DecriptIncremental(adjacent, _posInMap);
			sf::Vector2f adjNodePos(tileWidth / 2.f + tileWidth * _posInMap.y,
				tileHeight / 2.f + tileHeight * _posInMap.x);
			debugModeEdges.emplace_back(nodePos, adjNodePos);
			debugModeEdges.rbegin()->SetThickness(DEBUG_EDGE_LINE_THICKNESS);
			debugModeEdges.rbegin()->SetFillColor(sf::Color::Blue);
		}
	}
}

bool TileGraph::IsInitialized()
{
	return initialized;
}

const TileMap& TileGraph::GetTileMap() const
{
	return tileMap;
}

void TileGraph::FindShortestPath(const sf::Vector2f& pos1, const sf::Vector2f& pos2, std::list<Tile>& list) const
{
	LOG << info() << "Initiating shortest-path find";

	const Tile& tile1 = tileMap.GetTileAt(pos1), tile2 = tileMap.GetTileAt(pos2);
	if (tile1.GetToken() == Tile::Token::INVALID || tile2.GetToken() == Tile::Token::INVALID)
	{
		LOG << error() << "Trying to find the shortest path between two tiles where one or both of them are invalid";
		LOG << error() << "pos1: (" << pos1.x << ", " << pos2.y << "), pos2: (" << pos2.x << ", " << pos2.y << ")";

		throw "One or both tiles are invalid";
	}

	int index1 = NormalizeIncremental(EncriptIncremental(tile1.GetRowIndex(), tile1.GetColumnIndex())),
		index2 = NormalizeIncremental(EncriptIncremental(tile2.GetRowIndex(), tile2.GetColumnIndex()));
	if (shortestPathCache.find(index1) != shortestPathCache.end())
	{
		BuildPath(index1, index2, list);
	}
	else if(shortestPathCache.find(index2) != shortestPathCache.end())
	{
		BuildPath(index2, index1, list);
	}
	else // We build the sp graph for index1;
	{
		// Use Dijkstra's algo to calculate 
		// all shortest paths from src (arbitrarily index1)
		const int src = index1;
		std::set<int> visited;
		std::queue<int> toVisit;
		toVisit.push(src);
		std::map<int, std::pair<int, int>>& dests = shortestPathCache[src];
		while (toVisit.size() > 0)
		{
			int node = toVisit.front();
			toVisit.pop();
			visited.insert(node);

			// LOG << debug() << "Visiting node: " << node;

			std::set<int> adjacentNodes = adjacencyMap.find(node)->second;
			std::set<int>::const_iterator iter;
			for (iter = adjacentNodes.begin(); iter != adjacentNodes.end(); iter++)
			{
				int adjacent = *iter;
				auto iter = dests.find(adjacent);
				std::pair<int, int> score(node, node != src ? dests[node].second + 1 : 1);
				if (iter == dests.end() || (iter != dests.end() && iter->second.second > score.second))
				{
					// LOG << debug() << "New score for node: " << adjacent << " {" << score.first << ", " << score.second << "}";

					dests[adjacent] = score;
				}

				if (visited.find(adjacent) == visited.end())
				{
					toVisit.push(adjacent);
				}
			}
		}

		BuildPath(src, index2, list);
	}
}

void TileGraph::DecriptIncremental(int index, sf::Vector2i& posInMap) const
{
	int mapWidth = tileMap.GetMapWidth();
	posInMap.x = index / mapWidth;
	posInMap.y = index % mapWidth;
}

int TileGraph::EncriptIncremental(int i, int j) const
{
	int mapWidth = tileMap.GetMapWidth();
	return i * mapWidth + j;
}

int TileGraph::NormalizeIncremental(int query) const
{
	std::map<int, std::set<int>>::const_iterator iter = adjacencyMap.lower_bound(query);
	if (iter == adjacencyMap.end())
	{
		iter--;
	}
	// If the query doesn't exist (possibly a MAZE_TILE)
	// we look for a close existing node in the square surrounding it 
	// and then the square surrounding the later and so on 
	// TODO: this needs to be optimized as it re-searches sub-squares 
	if (iter->first != query) 
	{
		sf::Vector2i posInMap;
		DecriptIncremental(query, posInMap);
		int inc = 1;
		while (true) // Ops :(
		{
			int x = posInMap.x, y = posInMap.y;
			for (int _x = x - inc; _x <= min(x + inc, tileMap.GetMapHeight() - 1); _x++)
			{
				for (int _y = y - inc; _y <= min(y + inc, tileMap.GetMapWidth() - 1); _y++)
				{
					if (_x >= 0 && _y >= 0)
					{
						int q = EncriptIncremental(_x, _y);
						if (adjacencyMap.find(q) != adjacencyMap.end())
						{
							return q;
						}
					}
				}
			}

			inc++;
		}
	}

	return iter->first;
}

void TileGraph::BuildPath(int src, int dest, std::list<Tile>& list) const
{
	LOG << info() << "Building list from src:" << src << " to des: " << dest;

	sf::Vector2i tileIndex;
	for (int curr = dest; curr != src; curr = shortestPathCache[src][curr].first)
	{
		DecriptIncremental(curr, tileIndex);
		list.push_front(tileMap.GetTileAt(tileIndex.x, tileIndex.y));
	}
}

void TileGraph::Draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if(tileMap.IsDebugMode()) 
	{
		for (const Line& shape : debugModeEdges)
		{
			target.draw(shape, states);
		}
		for (const Point& shape : debugModeNodes)
		{
			target.draw(shape, states);
		}
	}
}
