#pragma once

#include "Screen.h"
#include "World/GameWorld.h"

class GamePlayScreen :
	public Screen
{
public:
	GamePlayScreen(Game& game);
	~GamePlayScreen() = default;

	virtual void Resize(const sf::Vector2u& size) override;

	virtual void Update(float deltaTime) override;

	virtual void Draw(sf::RenderTarget& target, sf::RenderStates states) const override;
private:
	GameWorld world;
};
