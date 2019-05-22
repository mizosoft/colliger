#pragma once

#include "SFML/Window/Event.hpp"

class Game;

template<typename ID, typename... Args>
class EventDispatcher;

using WindowEventDispatcher = EventDispatcher<sf::Event::EventType, sf::Event&, Game&>;
