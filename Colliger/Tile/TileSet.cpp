#include "TileSet.h"

#include <fstream>
#include "Helper/Utils/Math.h"
#include "Helper/Resource/PathResolver.h"
#include "JSON/json.hpp"
#include "JSON/json_fwd.hpp" // For JsonNode

const SimpleLogger TileSet::LOG = SimpleLogger("TileSet");
const sf::IntRect TileSet::REGION_NOT_FOUND = sf::IntRect();

TileSet::TileSet(const std::string& fileName, std::shared_ptr<PathResolver> resolver)
{
	LOG << info() << "Reading tileset file: " + fileName;

	std::ifstream fileStream(fileName);
	if (fileStream.is_open())
	{
		JsonNode js;
		fileStream >> js;

		// load texture
		std::string textureFileName = js["image"];
		LOG << info() << "Loading texture: " + textureFileName;
		std::shared_ptr<sf::Texture> _texture = std::make_shared<sf::Texture>();
		if (_texture->loadFromFile(resolver->Resolve(textureFileName)))
		{
			tileCount = js["tilecount"];
			imageWidth = js["imagewidth"];
			imageHeight = js["imageheight"];
			tileWidth = js["tilewidth"];
			tileHeight = js["tileheight"];
			margin = js["margin"];
			spacing = js["spacing"];
			texture = _texture;

			LOG << info() << "TileSet initialized successfully";
			LOG << debug() << "tileCount: " << tileCount;
			LOG << debug() << "imageWidth: " << imageWidth;
			LOG << debug() << "imageHeight: " << imageHeight;
			LOG << debug() << "tileWidth: " << tileWidth;
			LOG << debug() << "tileHeight: " << tileHeight;
			LOG << debug() << "margin: " << margin;
			LOG << debug() << "spacing: " << spacing;

			// Read invalidTileSetIndices
			/*JsonNode props = js["properties"];
			for (const JsonNode& prop : props)
			{
				if (prop["name"] == "invalidTileSetIndices")
				{
					for (int i : prop["value"])
					{
						invalidTileSetIndices.push_back(i);
					}
				}
			}*/
		}
		else
		{
			LOG << error() << "Couldn't load texture: " + textureFileName;

			throw "Couldn't load texture: " + textureFileName;
		}

		fileStream.close();
	}
	else
	{
		LOG << error() << "Couldn't open tileset file: " + fileName;

		throw "Couldn't open tileset file: " + fileName;
	}
}

sf::IntRect TileSet::FindRegion(int index) const
{
	sf::IntRect rect = REGION_NOT_FOUND;

	int _index = index - 1; // index is 1 based
	if (_index < tileCount && _index >= 0)
	{
		// Decript the individual row/column
		// indices of the region
		int rowIndex = _index / GetColumnCount();
		int columnIndex = _index % GetColumnCount();

		// Upper-left coords
		int x = margin + columnIndex * tileWidth + columnIndex * spacing;
		int y = margin + rowIndex * tileHeight + rowIndex * spacing;

		rect = sf::IntRect(x, y, tileWidth, tileHeight);
	}

	return rect;
}

int TileSet::GetTileWidth() const
{
	return tileWidth;
}

int TileSet::GetTileHeight() const
{
	return tileHeight;
}

int TileSet::GetRowCount() const
{
	return (imageHeight - 2 * margin + spacing) / (tileHeight + spacing);
}

int TileSet::GetColumnCount() const
{
	return (imageWidth - 2 * margin + spacing) / (tileWidth + spacing);
}

/*bool TileSet::IsInvalidIndex(int index) const
{
	for (int i : invalidTileSetIndices)
	{
		if (i == index)
		{
			return true;
		}
	}
	
	return false;
}*/

std::shared_ptr<sf::Texture> TileSet::GetTexture() const
{
	return texture;
}
