#include "Game.h"
#include "Helper/Resource/StatelessPathResolver.h"

const int Game::WIDTH = 912;
const int Game::HEIGHT = 624;
const int Game::TASKS_PER_THREAD = 10;
const SimpleLogger Game::LOG = SimpleLogger("Game");

Game::Game() :
	running(false), 
	window(sf::VideoMode(WIDTH, HEIGHT, sf::Style::Default), "Colliger"),
	eventDispatcher(),
	resourceManager(TASKS_PER_THREAD),
	screen(),
	addedLiseners()
{
	// Add top-level listeners
	addedLiseners.push_back(eventDispatcher.SubscribeCallback(
		sf::Event::Closed,
		[](sf::Event&, Game & game)
		{
			game.Exit();
		}
	));
	addedLiseners.push_back(eventDispatcher.SubscribeCallback(
		sf::Event::Resized,
		[](sf::Event& evt, Game & game)
		{
			std::shared_ptr<Screen> screen = game.GetScreen();
			if (screen)
			{
				screen->Resize({ evt.size.width, evt.size.height });
			}
		}
	));
	addedLiseners.push_back(eventDispatcher.SubscribeCallback(
		sf::Event::LostFocus,
		[](sf::Event & evt, Game & game)
		{
			std::shared_ptr<Screen> screen = game.GetScreen();
			if (screen)
			{
				screen->Pause();
			}
		}
	));
	addedLiseners.push_back(eventDispatcher.SubscribeCallback(
		sf::Event::GainedFocus,
		[](sf::Event & evt, Game & game)
		{
			std::shared_ptr<Screen> screen = game.GetScreen();
			if (screen)
			{
				screen->Resume();
			}
		}
	));

	// TODO: organize resource directories
	resourceManager.SetPathResolver(new StatelessPathResolver(
		[](const std::string & unresolved)
		{
			auto iter = std::find(unresolved.begin(), unresolved.end(), '.');
			if (iter != unresolved.end())
			{
				// std::string ext(iter, unresolved.end());
				// char (*lowerCase)(char) = std::tolower<std::string>;
				// std::transform(ext.begin(), ext.end(), ext.begin(), std::tolower);
			}

			return "Resources/" + unresolved;
		}
	));
}

Game::~Game()
{
	Exit();
}

void Game::Run()
{
	LOG << info() << "Starting the game loop";

	running = true;
	while (running && window.isOpen())
	{
		// Poll events
		sf::Event event;
		while (window.pollEvent(event))
		{
			eventDispatcher.Fire(event.type, event, *this);
		}

		// Clear prev shit
		window.clear(sf::Color::Black);

		// Update and draw the current screen
		sf::Time delta = clock.restart();
		if (screen)
		{
			screen->Update(delta.asSeconds());
			window.draw(*screen);
		}

		// Display the shit we drew
		window.display();
	}
}

void Game::Exit()
{
	running = false;
	window.close();
}

const sf::RenderWindow& Game::GetWindow() const
{
	return window;
}

WindowEventDispatcher& Game::GetEventDispatcher()
{
	return eventDispatcher;
}

ResourceManager& Game::GetResourceManager()
{
	return resourceManager;
}

std::shared_ptr<Screen> Game::GetScreen() const
{
	return screen;
}

void Game::SetScreen(std::shared_ptr<Screen> screen)
{
	this->screen = screen;

	sf::Vector2u windowSize(window.getSize());
	this->screen->Resize({ windowSize.x, windowSize.y });
}
