#pragma once

#include "Viewport.h"

class TryFitViewport :
	public Viewport
{
public:
	TryFitViewport(const sf::FloatRect& worldView = sf::FloatRect());
	~TryFitViewport() = default;

	virtual void Update(const sf::Vector2u& newWindowSize) override;
};
