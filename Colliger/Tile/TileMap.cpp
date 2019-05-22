#include <fstream>
//#include <algorithm>

#include "JSON/json.hpp"
#include "JSON/json_fwd.hpp" // For JsonNode
#include "Helper/Resource/PathResolver.h"
#include "TileMap.h"

const SimpleLogger TileMap::LOG = SimpleLogger("TileMap");

TileMap::TileMap(const std::string& fileName, std::shared_ptr<PathResolver> resolver)
{
	LOG << info() << "Reading tilemap file: " + fileName;

	std::ifstream fileStream(fileName);
	if (fileStream.is_open())
	{
		JsonNode js;
		fileStream >> js;

		bool infinite = js["infinite"];
		std::string orientation = js["orientation"];
		if (infinite || (orientation != "orthogonal"))
		{
			LOG << error() << "Unsupported tilemap: infinite=" << infinite << ", orientation=" << orientation;

			throw "Unsupported tilemap";
		}

		// Initialize width, height
		width = js["width"];
		height = js["height"];
		tileWidth = js["tilewidth"];
		tileHeight = js["tileheight"];

		// Load tileSets
		LOG << debug() << "Initializing tileSets";
		for (auto& tileSetObj : js["tilesets"])
		{
			tileSets.emplace(tileSetObj["firstgid"], TileSet(resolver->Resolve(tileSetObj["source"]), resolver));
		}

		// Get debugMode property
		debugMode = false;
		for (const auto& prop : js["properties"])
		{
			if (prop["name"] == "debugMode")
			{
				debugMode = prop["value"];

				break;
			}
		}

		// Initialize layers
		LOG << debug() << "Initializing tileLayers";

		for (const JsonNode& layerObj : js["layers"])
		{
			tileLayers.emplace_back(layerObj);
		}
		for (TileLayer& layer : tileLayers)
		{
			layer.InitializeTiles(tileWidth, tileHeight, tileSets);
			layer.InitializeDebugModeObjects();
			layer.SetDebugMode(debugMode);
		}

		fileStream.close();

		LOG << debug() << "Initialized the TileMap successfully";
	}
	else
	{
		LOG << error() << "Couldn't open file: " + fileName;

		throw "Couldn't open file: " + fileName;
	}
}

int TileMap::GetMapWidth() const
{
	return width;
}

int TileMap::GetMapHeight() const
{
	return height;
}

int TileMap::GetTileWidth() const
{
	return tileWidth;
}

int TileMap::GetTileHeight() const
{
	return tileHeight;
}

int TileMap::GetWidth() const
{
	return GetMapWidth() * GetTileWidth();
}

int TileMap::GetHeight() const
{
	return GetMapHeight() * GetTileHeight();
}

bool TileMap::IsDebugMode() const
{
	return debugMode;
}

const Tile& TileMap::GetTileAt(const sf::Vector2f& pos) const
{
	int i = static_cast<int>(pos.y / tileHeight), j = static_cast<int>(pos.x / tileWidth);
	for (const TileLayer& layer : tileLayers)
	{
		const std::vector<std::vector<Tile>>& tiles = layer.GetTiles();
		if (i < tiles.size() && j < tiles[i].size())
		{
			return tiles[i][j];
		}
	}

	return Tile::INVALID_TILE;
}

const Tile& TileMap::GetTileAt(int i, int j) const
{
	for (const TileLayer& layer : tileLayers)
	{
		const std::vector<std::vector<Tile>>& tiles = layer.GetTiles();
		if (i >= 0 && i < tiles.size() && j >= 0 && j < tiles[i].size())
		{
			return tiles[i][j];
		}
	}
	
	return Tile::INVALID_TILE;
}

const Tile& TileMap::GetTileAt(int index) const
{
	return GetTileAt(index / width, index % height);
}

const Tile& TileMap::GetNeighbooringTile(const Tile& tile, int rowInc, int columnInc) const
{
	if (tile.GetToken() == Tile::Token::INVALID)
	{
		return Tile::INVALID_TILE;
	}

	int i = tile.GetRowIndex(), j = tile.GetColumnIndex();
	for (const TileLayer& layer : tileLayers) // Lookup which layer owns the tile
	{
		const std::vector<std::vector<Tile>>& tiles = layer.GetTiles();
		if (i < tiles.size() && j < tiles[i].size()) // Index withIn bounds, may find a match
		{
			const Tile& tile = tiles[i][j];
			if (layer.GetId() == tile.GetLayerId()) // The Tile belongs to the TileLayer
			{
				int newI = i + rowInc, newJ = j + columnInc;
				return newI >= 0 && newI < tiles.size() && newJ >= 0 && newJ < tiles[newI].size() ?
					tiles[newI][newJ] : Tile::INVALID_TILE;
			}
		}
	}

	return Tile::INVALID_TILE;
}

const Tile& TileMap::GetLeftTile(const Tile& tile) const
{
	// std::cout << "--------" << tile.GetTileRectangle().left << ", " << tile.GetTileRectangle().top;
	return GetNeighbooringTile(tile, 0, -1);
}

const Tile& TileMap::GetRightTile(const Tile& tile) const
{
	return GetNeighbooringTile(tile, 0, 1);
}

const Tile& TileMap::GetUpperTile(const Tile& tile) const
{
	return GetNeighbooringTile(tile, -1, 0);
}

const Tile& TileMap::GetLowerTile(const Tile& tile) const
{
	return GetNeighbooringTile(tile, 1, 0);
}

const Tile& TileMap::GetNextPortal(const Tile& tile) const
{
	int index = tile.GetRowIndex() * width + height;
	auto iter = portals.find(index);
	if (iter != portals.end())
	{
		return GetTileAt(iter->second);
	}

	return Tile::INVALID_TILE;
}

const std::list<TileLayer>& TileMap::GetLayers() const
{
	return tileLayers;
}

void TileMap::Draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (const TileLayer& layer : tileLayers)
	{
		if (layer.IsVisible())
		{
			target.draw(layer, states);
		}
	}
}
