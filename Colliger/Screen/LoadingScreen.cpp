#include "LoadingScreen.h"

#include "SFML/Graphics/Sprite.hpp"
#include "Assets.h"
#include "GamePlayScreen.h"
#include "MenuScreen.h"
#include "Game.h"
#include "Helper/Resource/ResourceManager.h"

sf::Texture texture;
sf::Sprite sprite;

LoadingScreen::LoadingScreen(Game& game) :
	Screen(game),
	resourceManager(game.GetResourceManager()),
	progressBar({ 200.f, 300.f }, { 200.f, 300.f }),
	prevPercentage(0)
{
	progressBar.SetThickness(12.f);
	progressBar.SetFillColor(sf::Color::White);

	Assets::GAME_FONT.Enqueue(resourceManager);
	Assets::FOOD_TILE_SET.Enqueue(resourceManager);
	Assets::GHOST_TILE_SET.Enqueue(resourceManager);
	Assets::COLLIGER_TILESET.Enqueue(resourceManager);
	Assets::MAZE_TILE_MAP.Enqueue(resourceManager);
}

void LoadingScreen::Update(float deltaTime)
{
	int currPercentage = resourceManager.UpdateLoading();
	if (currPercentage != prevPercentage)
	{
		float perc = (float)currPercentage / ResourceManager::FULLY_LOADED;
		progressBar.Reset(progressBar.GetBeginning(), { 200.f + perc * 400.f, 300.f });
		prevPercentage = currPercentage;
	}

	if (prevPercentage == ResourceManager::FULLY_LOADED)
	{
		game.SetScreen(std::make_shared<MenuScreen>(game));
	}
}

void LoadingScreen::Draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(progressBar, states);
}
