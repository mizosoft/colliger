#pragma once

#include "SFML/Graphics/Drawable.hpp"
#include "SFML/Graphics/RenderTarget.hpp"
#include "SFML/Graphics/RenderStates.hpp"

/// This class only exists to conform to the
/// current convention of using PascalCase 
/// methods names on derived classes 
/// i know it's stupid but i'm a bit religious
/// so whatever lol
class Drawable : public sf::Drawable
{
public:
	~Drawable() = default;
protected:
	virtual void Draw(sf::RenderTarget& target, sf::RenderStates states) const = 0;

private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		Draw(target, states);
	}
};
