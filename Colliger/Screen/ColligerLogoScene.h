#pragma once

#include "SFML/Graphics/Text.hpp"
#include "DelayedScene.h"
#include "ColligerActor_1.h"

class ColligerLogoScene :
	public DelayedScene
{
public:
	ColligerLogoScene(const sf::Vector2u& worldBounds, ResourceManager& resourceManager);
	~ColligerLogoScene() = default;

	void Init(
		unsigned int characterWidth,
		float letterSpacing,
		float yScale,
		float colligerVelocity
	);

	sf::Text& GetText(); // Not const as the text is mutated

	virtual void Update(float deltaTime) override;

	virtual void Draw(sf::RenderTarget& target, sf::RenderStates states) const override;
private:
	const sf::Vector2u& worldBounds;
	ColligerActor_1 colliger;
	sf::Text text;
	float textVelocity;
	float colligerTargetX, textTargetX; // Centers of colliger, text after the animation is finished
};
