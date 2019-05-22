#include "TryFitViewport.h"

#include <cmath>

TryFitViewport::TryFitViewport(const sf::FloatRect& worldView) :
	Viewport(worldView)
{
}

void TryFitViewport::Update(const sf::Vector2u& newWindowSize)
{
	const sf::Vector2f& worldSize = view.getSize();

	sf::FloatRect viewport;
	float windowRatio = (float)newWindowSize.x / newWindowSize.y;
	float worldRatio = (float)worldSize.x / worldSize.y;
	if (windowRatio < worldRatio)
	{
		viewport.width = (float)newWindowSize.x;
		viewport.height = (float)std::floor(newWindowSize.x / worldRatio);
	}
	else
	{
		viewport.width = (float)std::floor(newWindowSize.y * worldRatio);
		viewport.height = (float)newWindowSize.y;
	}

	viewport.left = std::floor((newWindowSize.x - viewport.width) / 2.f);
	viewport.top = std::floor((newWindowSize.y - viewport.height) / 2.f);

	// SFML uses ratios
	viewport.width /= (float)newWindowSize.x;
	viewport.height /= (float)newWindowSize.y;
	viewport.left /= (float)newWindowSize.x;
	viewport.top /= (float)newWindowSize.y;

	view.setViewport(viewport);
	const sf::Vector2f & center = view.getCenter();
	view.setCenter(std::floor(center.x), std::floor(center.y));

	// std::cout << "VIEWPORT CHANGE: " << viewport.left << ", " << viewport.top << ", " << viewport.width << ", " << viewport.height << std::endl;

	// I have a feeling that i just got floored
}
