#pragma once

#include "Core/Drawable.h"
#include "Core/Updatable.h"
#include "Core/NoCopy.h"

class Game;

class Screen : 
	protected NoCopy,
	public Drawable, 
	public Updatable
{
public:
	Screen(Game& game);
	virtual ~Screen() = default;

	virtual void Resize(const sf::Vector2u& size) {};

	virtual void Pause();

	virtual void Resume();

	virtual const sf::View& GetView() const;

	virtual void Update(float deltaTime) = 0;

	virtual void Draw(sf::RenderTarget& target, sf::RenderStates states) const = 0;
protected:
	Game& game;
};
