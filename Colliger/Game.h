#pragma once

#include <memory>

#include "SFML/Window/Event.hpp"
#include "SFML/System/Clock.hpp"
#include "SFML/Graphics/RenderWindow.hpp"
#include "Helper/Resource/ResourceManager.h"
#include "Helper/Event/EventDispatcher.h"
#include "Helper/Logger/SimpleLogger.h"
#include "Helper/Event/WindowEventDispatcher.h"
#include "Helper/Event/WindowEventListener.h"
#include "Screen/Screen.h"

class Game
{
public:
	Game();
	~Game();

	void Run();

	void Exit();

	const sf::RenderWindow& GetWindow() const;

	WindowEventDispatcher& GetEventDispatcher();

	ResourceManager& GetResourceManager();

	std::shared_ptr<Screen> GetScreen() const;

	void SetScreen(std::shared_ptr<Screen> screen);
public:
	static const int WIDTH;
	static const int HEIGHT; 
private:
	static const int TASKS_PER_THREAD;
	static const SimpleLogger LOG;

	bool running;
	sf::Clock clock;
	sf::RenderWindow window;
	WindowEventDispatcher eventDispatcher;
	ResourceManager resourceManager;
	std::shared_ptr<Screen> screen;
	std::list<std::shared_ptr<WindowEventListener>> addedLiseners;
};
