#include "Screen.h"

#include "Game.h"

Screen::Screen(Game& game) : 
	game(game) 
{ 
}

void Screen::Pause()
{
}

void Screen::Resume()
{
}

const sf::View& Screen::GetView() const
{
	return game.GetWindow().getView();
}
