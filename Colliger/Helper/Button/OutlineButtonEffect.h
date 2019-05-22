#pragma once

#include "SFML/Graphics/Color.hpp"
#include "ButtonEffect.h"
#include "Button.h"

template<typename SpriteType>
class OutlineButtonEffect :
	public ButtonEffect
{
public:
	OutlineButtonEffect(float outlineThickness, const sf::Color& outlineColor, bool respectAlpha = false);
	~OutlineButtonEffect() = default;

	virtual void Apply(BaseButton& button) override;

	virtual void UnApply(BaseButton& button) override;
private:
	float outlineThickness;
	sf::Color outlineColor;

	float prevOutlineThickness;
	sf::Color prevOutlineColor;
	bool respectAlpha;
};

template<typename SpriteType>
inline OutlineButtonEffect<SpriteType>::OutlineButtonEffect(float outlineThickness, const sf::Color& outlineColor, bool respectAlpha) :
	outlineThickness(outlineThickness),
	outlineColor(outlineColor),
	prevOutlineThickness(0.f),
	prevOutlineColor(),
	respectAlpha(respectAlpha)
{
}

template<typename SpriteType>
inline void OutlineButtonEffect<SpriteType>::Apply(BaseButton& button)
{
	Button<SpriteType>& _button = dynamic_cast<Button<SpriteType>&>(button);
	SpriteType& sprite = _button.GetSprite();

	// Capture prev values
	prevOutlineThickness = sprite.getOutlineThickness();
	prevOutlineColor = sprite.getOutlineColor();

	// Set effect values
	sprite.setOutlineThickness(outlineThickness);
	if (respectAlpha)
	{
		const sf::Color& c = outlineColor; // For brevity
		sprite.setOutlineColor({ c.r, c.g, c.b, prevOutlineColor.a }); // Respect the alpha value
	}
	else
	{
		sprite.setOutlineColor(outlineColor);
	}
}

template<typename SpriteType>
inline void OutlineButtonEffect<SpriteType>::UnApply(BaseButton& button)
{
	Button<SpriteType>& _button = dynamic_cast<Button<SpriteType>&>(button);
	SpriteType& sprite = _button.GetSprite();

	sprite.setOutlineThickness(prevOutlineThickness);
	if (respectAlpha)
	{
		const sf::Color& c = outlineColor; // For brevity
		sf::Uint8 currAlpha = sprite.getOutlineColor().a;
		sprite.setOutlineColor({ c.r, c.g, c.b, currAlpha }); // Respect the alpha value
	}
	else
	{
		sprite.setOutlineColor(outlineColor);
	}
}
