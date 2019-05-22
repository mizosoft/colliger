#pragma once

#include "Core/Drawable.h"
#include "Core/Updatable.h"
#include "Tile/TileMap.h"
#include "Tile/TileGraph.h"
#include "Helper/Event/WindowEventDispatcher.h"
#include "Helper/Viewport/Viewport.h"
#include "Entity/Colliger.h"
#include "Entity/Ghost.h"

class ResourceManager;

class GameWorld :
	public Drawable,
	public Updatable
{
public:
	GameWorld(ResourceManager& resourceManager, WindowEventDispatcher& windowDispatcher);
	~GameWorld() = default;

	const TileMap& GetTileMap() const;

	const TileGraph& GetTileGraph() const;

	const sf::View& GetView() const;

	const Colliger& GetPlayer() const;

	void Resize(const sf::Vector2u& dimensions);

	virtual void Update(float deltaTime) override;

	virtual void Draw(sf::RenderTarget& target, sf::RenderStates states) const override;
private:
	Colliger colliger;
	std::list<Ghost> ghosts;
	std::shared_ptr<TileMap> tileMap;
	TileGraph tileGraph;
	std::unique_ptr<Viewport> viewport;
};
