#pragma once

#include "Screen.h"
#include "Helper/Shape/Line.h"

class ResourceManager;

class LoadingScreen :
	public Screen
{
public:
	LoadingScreen(Game& game);
	~LoadingScreen() = default;

	virtual void Update(float deltaTime) override;

	virtual void Draw(sf::RenderTarget& target, sf::RenderStates states) const override;
private:
	ResourceManager& resourceManager;
	Line progressBar;
	int prevPercentage;
};
