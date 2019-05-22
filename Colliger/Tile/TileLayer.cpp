#include "TileLayer.h"

#include <string>
#include <memory>

#include "SFML/Graphics/Sprite.hpp"
#include "SFML/Graphics/Texture.hpp"
#include "JSON/json.hpp"
#include "TileSet.h"

const SimpleLogger TileLayer::LOG = SimpleLogger("TileLayer");
float TileLayer::DEBUG_BORDER_THICKNESS = 1.f;
float TileLayer::DEBUG_CENTER_RADIUS = 1.f;
float TileLayer::DEBUG_CENTER_POINTS = 4.f;

TileLayer::TileLayer(const JsonNode& layerObj) :
	debugMode(false)
{
	id = layerObj["id"];
	LOG << info() << "Initializing TileLayer: " << id;

	const std::string& type = layerObj["type"];
	if (type != "tilelayer")
	{
		LOG << error() << "Unsupported layer type: " + type;

		throw "Unsupported layer type: " + type;
	}

	int width, height;
	width = layerObj["width"];
	height = layerObj["height"];
	const JsonNode& data = layerObj["data"];
	// Initialize tileIndices maping each (x, y) 
	// tile location to it's image index in the TileSet
	for (int i = 0; i < height; i++)
	{
		tileIndices.emplace_back();
		for (int j = 0; j < width; j++)
		{
			tileIndices[i].push_back(data[i * width + j]);
		}
	}
	
	visible = layerObj["visible"];
}

void TileLayer::InitializeTiles(int tileWidth, int tileHeight, const std::map<int, TileSet>& tileSets)
{
	LOG << info() << "Initializing tiles in layer: " << id;

	int width = GetLayerWidth(), height = GetLayerHeight();
	// Initialize the two dimensional Tile array representing the layer
	for (int i = 0; i < height; i++)
	{
		tiles.emplace_back();
		for (int j = 0; j < width; j++)
		{
			int index = tileIndices[i][j];

			LOG << debug() << "Initializing tile (" << i << ", " << j << "), of index " << index;

			sf::Vector2f tilePosition(static_cast<float>(j * tileWidth), static_cast<float>(i * tileHeight));
			if(index > 0) // Filled tile, need to lookup the index in the TileSet to create the Sprite
			{
				std::map<int, TileSet>::const_iterator it = tileSets.lower_bound(index);
				if ((it == tileSets.end() && tileSets.size() > 0) || 
					(it->first > index && it != tileSets.begin()))
				{
					it--;
				}
				if (it->first <= index)
				{
					const TileSet& tileSet = it->second;
					sf::IntRect region = tileSet.FindRegion(index);
					if (region != TileSet::REGION_NOT_FOUND)
					{
						sf::Sprite sprite(*it->second.GetTexture(), region);
						sprite.setPosition(tilePosition);
						sprite.setScale((float)tileWidth / tileSet.GetTileWidth(), (float)tileHeight / tileSet.GetTileHeight());
						tiles[i].push_back(Tile(id, i, tiles[i].size(), sprite));
					}
					else
					{
						LOG << warn() << "Region of index: " << index << " not found";
					}
				}
				else
				{
					LOG << warn() << "TileSet containing index: " << index << " not found";
				}
			}
			else if(index == 0) // Empty tile
			{
				tiles[i].push_back(Tile(id, i, tiles[i].size(),
					sf::FloatRect(tilePosition.x, tilePosition.y, 
						static_cast<float>(tileWidth), static_cast<float>(tileHeight))));
			}
			else
			{
				LOG << warn() << "Illegal tile index value: " + index;
			}
		}
	}
}

bool TileLayer::IsVisible() const
{
	return visible;
}

int TileLayer::GetId() const
{
	return id;
}

int TileLayer::GetLayerWidth() const
{
	return tileIndices.size() > 0 ? tileIndices[0].size() : 0;
}

int TileLayer::GetLayerHeight() const
{
	return tileIndices.size();
}

const std::vector<std::vector<Tile>>& TileLayer::GetTiles() const
{
	return tiles;
}

void TileLayer::Draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (int i = 0; i < tiles.size(); i++)
	{
		for (int j = 0; j < tiles[i].size(); j++)
		{
			target.draw(tiles[i][j], states);
			if (debugMode)
			{
				target.draw(debugTileBorders[i][j], states);
				target.draw(debugTileCenters[i][j], states);
			}
		}
	}
}

void TileLayer::SetDebugMode(bool val)
{
	debugMode = val;
}

void TileLayer::InitializeDebugModeObjects()
{
	LOG << info() << "Initializing debug mode objects...";

	debugTileBorders.clear();
	debugTileCenters.clear();
	for (int i = 0; i < tiles.size(); i++)
	{
		debugTileBorders.push_back(std::vector<sf::RectangleShape>());
		debugTileCenters.push_back(std::vector<sf::CircleShape>());
		for (int j = 0; j < tiles[i].size(); j++)
		{
			const sf::FloatRect& tileRect = tiles[i][j].GetTileRectangle();
			debugTileBorders[i].emplace_back(sf::Vector2f(tileRect.width, tileRect.height));
			debugTileBorders[i][j].setPosition(sf::Vector2f(tileRect.left, tileRect.top));
			debugTileBorders[i][j].setFillColor(sf::Color::Transparent);
			debugTileBorders[i][j].setOutlineColor(sf::Color::Red);
			debugTileBorders[i][j].setOutlineThickness(DEBUG_BORDER_THICKNESS);

			debugTileCenters[i].emplace_back(DEBUG_CENTER_RADIUS);
			debugTileCenters[i][j].setFillColor(sf::Color::Red);
			sf::Vector2f tileCenter = tiles[i][j].GetTileCenter();
			debugTileCenters[i][j].setPosition(tileCenter.x - DEBUG_CENTER_RADIUS, tileCenter.y - DEBUG_CENTER_RADIUS);
		}
	}
}
