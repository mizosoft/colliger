/*#include "ScaleButtonEffect.h"

#include "Button.h"
#include "SFML/Graphics/Transformable.hpp"

ScaleButtonEffect::ScaleButtonEffect(sf::Vector2f scale) :
	scale(scale),
	prevScale()
{
}

void ScaleButtonEffect::Apply(BaseButton& button)
{
	Button<sf::Transformable>& _button = dynamic_cast<Button<sf::Transformable>&>(button);
	sf::Transformable& sprite = _button.GetSprite();
	prevScale = sprite.getScale();
	sprite.setScale(scale);
}

void ScaleButtonEffect::UnApply(BaseButton& button)
{
	Button<sf::Transformable>& _button = dynamic_cast<Button<sf::Transformable>&>(button);
	_button.GetSprite().setScale(prevScale);
}*/
