#pragma once

#include "Helper/Resource/ResourceManager.h"

template<typename T>
struct Asset
{
	const std::string name;

	inline Asset(const std::string& name) :
		name(name)
	{
	}

	inline void Enqueue(ResourceManager& resourceManager) const
	{
		resourceManager.Enqueue<T>(name);
	}

	inline std::shared_ptr<T> Get(ResourceManager& resourceManager) const
	{
		return resourceManager.GetResource<T>(name);
	}
};

struct Assets
{
	static const Asset<sf::Font> GAME_FONT;
	static const Asset<TileSet> FOOD_TILE_SET;
	static const Asset<TileSet> GHOST_TILE_SET;
	static const Asset<TileMap> MAZE_TILE_MAP;
	static const Asset<TileSet> COLLIGER_TILESET;
};
