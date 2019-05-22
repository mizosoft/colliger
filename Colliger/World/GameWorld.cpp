#include "GameWorld.h"

#include "Helper/Viewport/TryFitViewport.h"
#include "Helper/Resource/ResourceManager.h"
#include "Entity/AmbushGhostBehaviour.h"

#include "Assets.h"

GameWorld::GameWorld(ResourceManager& resourceManager, WindowEventDispatcher& windowDispatcher) :
	colliger(resourceManager, windowDispatcher, *this),
	viewport(new TryFitViewport()),
	tileMap(Assets::MAZE_TILE_MAP.Get(resourceManager)),
	tileGraph(*tileMap)
{
	tileGraph.InitializeGraph();
	tileGraph.InitializeDebugModeObjects();

	viewport->GetView().reset({ 0.f, 0.f, (float)tileMap->GetWidth(), (float)tileMap->GetHeight() });

	colliger.InitializeSprite();
	colliger.FixOriginOnCenter();
	colliger.SetCenterPosition(tileMap->GetTileAt(17, 18).GetTileCenter());

	ghosts.emplace_back(GhostStyle::RED, new AmbushGhostBehaviour(), resourceManager, *this);
	Ghost& ghost = ghosts.back();
	ghost.SetState(GhostState::CHASING);
	ghost.InitializeSprite();
	ghost.SetCenterPosition(tileMap->GetTileAt(17, 2).GetTileCenter());
}

const TileMap& GameWorld::GetTileMap() const
{
	return *tileMap;
}

const TileGraph& GameWorld::GetTileGraph() const
{
	return tileGraph;
}

const sf::View& GameWorld::GetView() const
{
	return viewport->GetView();
}

const Colliger& GameWorld::GetPlayer() const
{
	return colliger;
}

void GameWorld::Resize(const sf::Vector2u& dimensions)
{
	viewport->Update(dimensions);

	// std::cout << "VIEWPORT CHANGE: " << viewport.left << ", " << viewport.top << ", " << viewport.width << ", " << viewport.height << std::endl;
}

void GameWorld::Update(float deltaTime)
{
	colliger.Update(deltaTime);
	for (Ghost& ghost : ghosts)
	{
		ghost.Update(deltaTime);
	}
}

void GameWorld::Draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.setView(viewport->GetView());
	target.draw(*tileMap, states);
	target.draw(tileGraph, states);
	for (const Ghost& ghost : ghosts)
	{
		target.draw(ghost, states);
	}
	target.draw(colliger, states);
	target.setView(target.getDefaultView());
}
