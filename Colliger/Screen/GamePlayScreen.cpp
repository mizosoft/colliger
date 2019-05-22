#include "GamePlayScreen.h"
#include "Game.h"

sf::View view;

GamePlayScreen::GamePlayScreen(Game& game) :
	Screen(game),
	world(game.GetResourceManager(), game.GetEventDispatcher())
{
	sf::Vector2u windowSize = game.GetWindow().getSize();
}

void GamePlayScreen::Resize(const sf::Vector2u& size)
{	
	world.Resize(size);
}

void GamePlayScreen::Update(float deltaTime)
{
	world.Update(deltaTime);
}

void GamePlayScreen::Draw(sf::RenderTarget& target, sf::RenderStates states) const
{	
	target.draw(world, states);
}
