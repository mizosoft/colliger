/*#include "Button.h"
#include "Game.h"

const std::pair<Button::PointerType, int> Button::NOT_PRESSED = std::make_pair(Button::PointerType::NONE, -1);

Button::Button(const CoordProjector& projector) :
	active(false),
	hoveredOver(false),
	pressedPointer(NOT_PRESSED),
	sprite(),
	originalSpriteBounds(),
	projector(projector),
	effects(),
	listeners(),
	addedWindowListeners()
{
}

void Button::Register(WindowEventDispatcher& windowDispatcher)
{
	AddHoverListeners(windowDispatcher);
	AddMouseClickListeners(windowDispatcher);
	AddTouchListeners(windowDispatcher);
}

void Button::SetActive(bool active)
{
	this->active = active;
}

void Button::SetButtonEffect(ButtonEffect::EffectType type, std::shared_ptr<ButtonEffect> effect)
{
	effects[type] = effect;
}

void Button::InvokeListeners()
{
	if (active)
	{
		for (ButtonListener& listener : listeners)
		{
			listener();
		}
	}
}

sf::Sprite& Button::GetSprite()
{
	return sprite;
}

void Button::SetSprite(const sf::Sprite& sprite)
{
	this->sprite = sprite;
	originalSpriteBounds = sprite.getGlobalBounds();
}

void Button::Draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(sprite, states);

	for (const Point& pnt : touchPoints)
	{
		target.draw(pnt, states);
	}
}

bool Button::Contains(const sf::Vector2f& posInWorld)
{
	return originalSpriteBounds.contains(posInWorld);
}

void Button::Pressed(const sf::Vector2f& posInWorld, PointerType pointerType, int button)
{
	// First click within range ?
	if (pressedPointer.first == PointerType::NONE && Contains(posInWorld))
	{
		pressedPointer = { pointerType, button };

		auto iter = effects.find(ButtonEffect::EffectType::CLICK);
		if (iter != effects.end())
		{
			iter->second->Apply(*this);
		}
	}
}

void Button::Released(const sf::Vector2f& posInWorld, PointerType pointerType, int button)
{
	// Was Pressed before so i'm interested releases (of the same type and button)
	if (pressedPointer.first != PointerType::NONE && pressedPointer == std::make_pair(pointerType, button))
	{
		// With in bounds so we trigger the listeners
		if (Contains(posInWorld))
		{
			switch (pressedPointer.first)
			{
			case MOUSE:
				if (static_cast<sf::Mouse::Button>(pressedPointer.second) == sf::Mouse::Button::Left) // Only left button triggers
				{
					InvokeListeners();
				}

				break;
			case TOUCH:
				InvokeListeners(); // Any finger is allowed (Feels weird saying it that way)
			default:
			{} // Impossible ??
			}

			Point pnt(posInWorld);
			pnt.SetFillColor(sf::Color::Green);
			touchPoints.push_back(pnt);
		}
		else
		{
			Point pnt(posInWorld);
			pnt.SetFillColor(sf::Color::Red);
			touchPoints.push_back(pnt);
		}
		
		// The effect is unapplied anyways
		auto iter = effects.find(ButtonEffect::EffectType::CLICK);
		if (iter != effects.end())
		{
			iter->second->UnApply(*this);
		}
		// Reset pressedPointer
		pressedPointer = NOT_PRESSED;
	}
}

void Button::MouseEnteredRegion()
{
	auto iter = effects.find(ButtonEffect::EffectType::HOVER);
	if (iter != effects.end())
	{
		iter->second->Apply(*this);
	}
}

void Button::MouseLeftRegion()
{
	auto iter = effects.find(ButtonEffect::EffectType::HOVER);
	if (iter != effects.end())
	{
		iter->second->UnApply(*this);
	}
}

// Look at all these gay lambdas *angry emoji*

void Button::AddHoverListeners(WindowEventDispatcher& windowDispatcher)
{
	addedWindowListeners.push_back(
		windowDispatcher.SubscribeCallback(
			sf::Event::EventType::MouseMoved,
			[this](sf::Event & evt, Game & game)
			{
				sf::Vector2f pos = projector(game, { evt.mouseMove.x, evt.mouseMove.y });
				if (Contains(pos) && !hoveredOver)
				{
					MouseEnteredRegion();
				}
				else if (hoveredOver)
				{
					MouseLeftRegion();
				}
			}
		)
	);
}

void Button::AddMouseClickListeners(WindowEventDispatcher& windowDispatcher)
{
	addedWindowListeners.push_back(
		windowDispatcher.SubscribeCallback(
			sf::Event::EventType::MouseButtonPressed,
			[this](sf::Event & evt, Game & game)
			{ 
				sf::Vector2f pos = projector(game, { evt.mouseMove.x, evt.mouseMove.y });
				Pressed(pos, PointerType::MOUSE, evt.mouseButton.button);
			}
		)
	);
	addedWindowListeners.push_back(
		windowDispatcher.SubscribeCallback(
			sf::Event::EventType::MouseButtonReleased,
			[this](sf::Event & evt, Game & game)
			{
				sf::Vector2f pos = projector(game, { evt.mouseMove.x, evt.mouseMove.y });
				Released(pos, PointerType::MOUSE, evt.mouseButton.button);
			}
		)
	);
}

void Button::AddTouchListeners(WindowEventDispatcher& windowDispatcher)
{
	addedWindowListeners.push_back(
		windowDispatcher.SubscribeCallback(
			sf::Event::EventType::TouchBegan,
			[this](sf::Event & evt, Game & game)
			{
				sf::Vector2f pos = projector(game, { evt.mouseMove.x, evt.mouseMove.y });
				Pressed(pos, PointerType::TOUCH, evt.touch.finger);
			}
		)
	);
	addedWindowListeners.push_back(
		windowDispatcher.SubscribeCallback(
			sf::Event::EventType::TouchEnded,
			[this](sf::Event & evt, Game & game)
			{
				sf::Vector2f pos = projector(game, { evt.mouseMove.x, evt.mouseMove.y });
				Released(pos, PointerType::TOUCH, evt.touch.finger);
			}
		)
	);
}*/
