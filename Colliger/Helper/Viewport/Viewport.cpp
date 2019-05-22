#include "Viewport.h"

Viewport::Viewport(const sf::FloatRect& worldView) :
	view(worldView)
{
}

sf::View& Viewport::GetView()
{
	return view;
}
