#pragma once

#include <string>
#include <memory>

#include "SFML/Graphics/Texture.hpp"
#include "Helper/Logger/SimpleLogger.h"

class PathResolver;

class TileSet
{
public:
	TileSet(const std::string& fileName, std::shared_ptr<PathResolver> resolver);
	~TileSet() = default;

	sf::IntRect FindRegion(int index) const;

	int GetTileWidth() const;

	int GetTileHeight() const;

	int GetRowCount() const;

	int GetColumnCount() const;

	// bool IsInvalidIndex(int index) const;

	std::shared_ptr<sf::Texture> GetTexture() const;
public:
	const static SimpleLogger LOG;
	const static sf::IntRect REGION_NOT_FOUND;
private:
	int tileCount;
	int tileWidth, tileHeight;
	int margin, spacing;
	int imageWidth, imageHeight;
	std::shared_ptr<sf::Texture> texture;
	// std::vector<int> invalidTileSetIndices;
};
