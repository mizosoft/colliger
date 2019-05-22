#pragma once

#include <functional>
#include <list>
#include <memory>
#include <map>

#include "Core/Drawable.h"
#include "Helper/Event/WindowEventDispatcher.h"
#include "Helper/Event/WindowEventListener.h"
#include "ButtonEffect.h"

#include "Helper/Shape/Point.h"

class BaseButton :
	public Drawable
{
public:
	using ButtonListener = std::function<void(void)>;
	using CoordProjector = std::function<sf::Vector2f(const Game&, const sf::Vector2i&)>;
public:
	BaseButton(const CoordProjector& projector);
	virtual ~BaseButton() = default;

	void Register(WindowEventDispatcher& windowDispatcher);

	void Unregister();

	void SetActive(bool active);

	template<typename Func>
	void AddListener(Func&& func);

	void SetButtonBounds(const sf::FloatRect& buttonBounds);

	sf::FloatRect GetButtonBounds() const;

	void SetButtonEffect(ButtonEffect::EffectType type, std::shared_ptr<ButtonEffect> effect);

	virtual void Draw(sf::RenderTarget& target, sf::RenderStates states) const = 0;
private:
	enum PointerType
	{
		NONE,
		MOUSE,
		TOUCH
	};

	bool Contains(const sf::Vector2f& posInWorld);

	void Pressed(const sf::Vector2f& posInWorld, PointerType pointerType, int button);

	void Released(const sf::Vector2f& posInWorld, PointerType pointerType, int button);

	void MouseEnteredRegion();

	void MouseLeftRegion();

	void AddHoverListeners(WindowEventDispatcher& windowDispatcher);

	void AddMouseClickListeners(WindowEventDispatcher& windowDispatcher);

	void AddTouchListeners(WindowEventDispatcher& windowDispatcher);

	void InvokeListeners();
private:
	static const std::pair<PointerType, int> NOT_PRESSED;

	bool active;
	bool hoveredOver;
	std::pair<PointerType, int> pressedPointer;
	sf::FloatRect buttonBounds;
	CoordProjector projector;
	std::map<ButtonEffect::EffectType, std::shared_ptr<ButtonEffect>> effects;
	std::list<ButtonListener> listeners;
	std::list<std::shared_ptr<WindowEventListener>> addedWindowListeners;
	
protected:
	// TEMP
	std::list<Point> touchPoints;
	// TEMP
};

template<typename Func>
inline void BaseButton::AddListener(Func&& func)
{
	listeners.emplace_back(std::forward<Func>(func));
}
