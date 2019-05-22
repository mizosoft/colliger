#pragma once

#include "SFML/System/Vector2.hpp"
#include "ButtonEffect.h"
#include "Button.h"

template<typename SpriteType>
class ScaleButtonEffect :
	public ButtonEffect
{
public:
	ScaleButtonEffect(sf::Vector2f scale);
	~ScaleButtonEffect() = default;

	virtual void Apply(BaseButton& button) override;

	virtual void UnApply(BaseButton& button) override;
private:
	sf::Vector2f scale;
	sf::Vector2f prevScale;
};

template<typename SpriteType>
ScaleButtonEffect<SpriteType>::ScaleButtonEffect(sf::Vector2f scale) :
	scale(scale),
	prevScale()
{
}

template<typename SpriteType>
void ScaleButtonEffect<SpriteType>::Apply(BaseButton& button)
{
	Button<SpriteType>& _button = dynamic_cast<Button<SpriteType>&>(button);
	SpriteType& sprite = _button.GetSprite();
	prevScale = sprite.getScale();
	sprite.setScale(scale);
}

template<typename SpriteType>
void ScaleButtonEffect<SpriteType>::UnApply(BaseButton& button)
{
	Button<SpriteType>& _button = dynamic_cast<Button<SpriteType>&>(button);
	_button.GetSprite().setScale(prevScale);
}
