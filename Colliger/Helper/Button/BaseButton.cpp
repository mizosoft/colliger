#include "BaseButton.h"
#include "Game.h"

const std::pair<BaseButton::PointerType, int> BaseButton::NOT_PRESSED = std::make_pair(BaseButton::PointerType::NONE, -1);

BaseButton::BaseButton(const CoordProjector & projector) :
	active(false),
	hoveredOver(false),
	pressedPointer(NOT_PRESSED),
	buttonBounds(),
	projector(projector),
	effects(),
	listeners(),
	addedWindowListeners(),
	touchPoints()
{
}

void BaseButton::Register(WindowEventDispatcher& windowDispatcher)
{
	AddHoverListeners(windowDispatcher);
	AddMouseClickListeners(windowDispatcher);
	AddTouchListeners(windowDispatcher);
}

void BaseButton::Unregister()
{
	addedWindowListeners.clear();
}

void BaseButton::SetActive(bool active)
{
	this->active = active;
}

void BaseButton::SetButtonBounds(const sf::FloatRect& buttonBounds)
{
	this->buttonBounds = buttonBounds;
}

sf::FloatRect BaseButton::GetButtonBounds() const
{
	return buttonBounds;
}

void BaseButton::SetButtonEffect(ButtonEffect::EffectType type, std::shared_ptr<ButtonEffect> effect)
{
	effects[type] = effect;
}

void BaseButton::InvokeListeners()
{
	if (active)
	{
		//std::cout << "Invoking " << listeners.size() << " listeners" << std::endl;
		for (ButtonListener& listener : listeners)
		{
			//std::cout << "Invoking listener" << std::endl;
			listener();
		}
	}
}

bool BaseButton::Contains(const sf::Vector2f& posInWorld)
{
	return buttonBounds.contains(posInWorld);
}

void BaseButton::Pressed(const sf::Vector2f& posInWorld, PointerType pointerType, int button)
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

void BaseButton::Released(const sf::Vector2f& posInWorld, PointerType pointerType, int button)
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

			// TEMP
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

void BaseButton::MouseEnteredRegion()
{
	auto iter = effects.find(ButtonEffect::EffectType::HOVER);
	if (iter != effects.end())
	{
		iter->second->Apply(*this);
	}
}

void BaseButton::MouseLeftRegion()
{
	auto iter = effects.find(ButtonEffect::EffectType::HOVER);
	if (iter != effects.end())
	{
		iter->second->UnApply(*this);
	}
}

// Look at all these gay lambdas *angry emoji*

void BaseButton::AddHoverListeners(WindowEventDispatcher& windowDispatcher)
{
	addedWindowListeners.push_back(
		windowDispatcher.SubscribeCallback(
			sf::Event::EventType::MouseMoved,
			[this](sf::Event & evt, Game & game)
			{
				sf::Vector2f pos = projector(game, { evt.mouseMove.x, evt.mouseMove.y });
				if (Contains(pos))
				{
					if (!hoveredOver)
					{
						MouseEnteredRegion();
						hoveredOver = true;
					}
				}
				else if(hoveredOver)
				{
					MouseLeftRegion();
					hoveredOver = false;
				}
			}
		)
	);
}

void BaseButton::AddMouseClickListeners(WindowEventDispatcher& windowDispatcher)
{
	addedWindowListeners.push_back(
		windowDispatcher.SubscribeCallback(
			sf::Event::EventType::MouseButtonPressed,
			[this](sf::Event & evt, Game & game)
			{
				sf::Vector2f pos = projector(game, { evt.mouseButton.x, evt.mouseButton.y });
				Pressed(pos, PointerType::MOUSE, evt.mouseButton.button);
			}
		)
	);
	addedWindowListeners.push_back(
		windowDispatcher.SubscribeCallback(
			sf::Event::EventType::MouseButtonReleased,
			[this](sf::Event & evt, Game & game)
			{
				sf::Vector2f pos = projector(game, { evt.mouseButton.x, evt.mouseButton.y });
				Released(pos, PointerType::MOUSE, evt.mouseButton.button);
			}
		)
	);
}

void BaseButton::AddTouchListeners(WindowEventDispatcher& windowDispatcher)
{
	addedWindowListeners.push_back(
		windowDispatcher.SubscribeCallback(
			sf::Event::EventType::TouchBegan,
			[this](sf::Event & evt, Game & game)
			{
				sf::Vector2f pos = projector(game, { evt.touch.x, evt.touch.y });
				Pressed(pos, PointerType::TOUCH, evt.touch.finger);
			}
		)
	);
	addedWindowListeners.push_back(
		windowDispatcher.SubscribeCallback(
			sf::Event::EventType::TouchEnded,
			[this](sf::Event & evt, Game & game)
			{
				sf::Vector2f pos = projector(game, { evt.touch.x, evt.touch.y });
				Released(pos, PointerType::TOUCH, evt.touch.finger);
			}
		)
	);
}
