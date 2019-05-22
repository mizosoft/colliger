#pragma once

#include <list>
#include <map>
#include <memory>

#include "SFML/Graphics.hpp"
#include "SFML/System/Vector2.hpp"
#include "SFML/Graphics/Shape.hpp"
#include "Core/Drawable.h"
#include "Helper/Logger/SimpleLogger.h"
#include "TileSet.h"
#include "TileLayer.h"

class PathResolver;

class TileMap : 
	public Drawable
{
public:
	TileMap(const std::string& fileName, std::shared_ptr<PathResolver> resolver);
	~TileMap() = default;

	int GetMapWidth() const;

	int GetMapHeight() const;

	int GetTileWidth() const;

	int GetTileHeight() const;

	int GetWidth() const;

	int GetHeight() const;

	bool IsDebugMode() const;

	const Tile& GetTileAt(const sf::Vector2f& pos) const;

	const Tile& GetTileAt(int i, int j) const;

	const Tile& GetTileAt(int index) const;

	const Tile& GetLeftTile(const Tile& tile) const;

	const Tile& GetRightTile(const Tile& tile) const;

	const Tile& GetUpperTile(const Tile& tile) const;

	const Tile& GetLowerTile(const Tile& tile) const;

	const Tile& GetNextPortal(const Tile& tile) const;

	const std::list<TileLayer>& GetLayers() const;

	virtual void Draw(sf::RenderTarget& target, sf::RenderStates states) const override;
private:
	const Tile& GetNeighbooringTile(const Tile& tile, int rowInc, int columnInc) const;
public:
	static const SimpleLogger LOG;
private:
	bool debugMode;
	int width, height;
	int tileWidth, tileHeight;
	std::map<int, TileSet> tileSets;
	std::list<TileLayer> tileLayers;
	std::map<int, int> portals;
};
