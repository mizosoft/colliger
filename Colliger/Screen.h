#pragma once

#include "Drawable.h"
#include "Updatable.h"

class Game;

class Screen : public Drawable, public Updatable
{
protected:
	Game &game;

public:
	Screen(Game &_game) : game(_game) { };
	virtual ~Screen() { }

	virtual void Pause() = 0;

	virtual void Resume() = 0;

	virtual void Resize(unsigned int width, unsigned int height) = 0;
};
