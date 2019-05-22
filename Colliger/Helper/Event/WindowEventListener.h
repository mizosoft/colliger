#pragma once

#include "SFML/Window/Event.hpp"
#include "EventListener.h"

class Game;

using WindowEventListener = EventListener<sf::Event&, Game&>;
