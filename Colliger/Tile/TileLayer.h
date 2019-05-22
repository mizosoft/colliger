#pragma once

#include <vector>
#include <map>

#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/Graphics/CircleShape.hpp"
#include "SFML/Graphics/VertexArray.hpp"
#include "Core/Drawable.h"
#include "Helper/Logger/SimpleLogger.h"
#include "JSON/json_fwd.hpp"
#include "Tile.h"

class TileSet;

class TileLayer : 
	public Drawable
{
	friend class TileMap;
public:
	TileLayer(const JsonNode& layerObj);
	~TileLayer() = default;

	void InitializeTiles(int tileWidth, int tileHeight, const std::map<int, TileSet>& tileSets);

	bool IsVisible() const;

	int GetId() const;

	int GetLayerWidth() const;

	int GetLayerHeight() const;

	const std::vector<std::vector<Tile>>& GetTiles() const;

	virtual void Draw(sf::RenderTarget& target, sf::RenderStates states) const override;
private:
	void SetDebugMode(bool val);

	void InitializeDebugModeObjects();
public:
	static const SimpleLogger LOG;
	static float DEBUG_BORDER_THICKNESS;
	static float DEBUG_CENTER_RADIUS;
	static float DEBUG_CENTER_POINTS;
private:
	int id;
	bool visible;
	bool debugMode;
	std::vector<std::vector<int>> tileIndices;
	std::vector<std::vector<Tile>> tiles;

	// debug objects
	std::vector<std::vector<sf::RectangleShape>> debugTileBorders;
	std::vector<std::vector<sf::CircleShape>> debugTileCenters;
};
