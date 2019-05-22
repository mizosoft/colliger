#pragma once

#include "BaseButton.h"

template<typename SpriteType>
class Button :
	public BaseButton
{
public:
	Button(const CoordProjector& projector);
	~Button() = default;

	void SetSprite(const SpriteType& sprite);

	SpriteType& GetSprite();

	virtual void Draw(sf::RenderTarget& target, sf::RenderStates states) const override;
private:
	SpriteType sprite;
};

template<typename SpriteType>
inline Button<SpriteType>::Button(const CoordProjector& projector) :
	BaseButton(projector),
	sprite()
{
}

template<typename SpriteType>
inline void Button<SpriteType>::SetSprite(const SpriteType& sprite)
{
	this->sprite = sprite;
	SetButtonBounds(this->sprite.getGlobalBounds()); // Appply bounds with new origin
}

template<typename SpriteType>
inline SpriteType& Button<SpriteType>::GetSprite()
{
	return sprite;
}

template<typename SpriteType>
inline void Button<SpriteType>::Draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(sprite, states);
}

/*#include <functional>
#include <list>
#include <memory>
#include <map>

#include "SFML/Graphics/Sprite.hpp"
#include "Core/Drawable.h"
#include "Helper/Event/WindowEventDispatcher.h"
#include "Helper/Event/WindowEventListener.h"
#include "ButtonEffect.h"
#include "Helper/Shape/Point.h"

class Button :
	public Drawable
{
public:
	using ButtonListener = std::function<void(void)>;
	using CoordProjector = std::function<sf::Vector2f(const Game&, const sf::Vector2i&)>;
public:
	Button(const CoordProjector& projector);
	~Button() = default;

	void Register(WindowEventDispatcher& windowDispatcher);
	
	void SetActive(bool active);

	template<typename Func>
	void AddListener(Func&& func);

	void SetButtonEffect(ButtonEffect::EffectType type, std::shared_ptr<ButtonEffect> effect);

	void InvokeListeners();

	sf::Sprite& GetSprite();

	void SetSprite(const sf::Sprite& sprite);

	virtual void Draw(sf::RenderTarget& target, sf::RenderStates states) const override;
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
private:
	static const std::pair<PointerType, int> NOT_PRESSED;

	bool active;
	bool hoveredOver;
	std::pair<PointerType, int> pressedPointer;
	sf::Sprite sprite;
	// Keep a ref to the initial sprite bounds as it might change in scalling effects
	sf::FloatRect originalSpriteBounds;
	CoordProjector projector;
	std::map<ButtonEffect::EffectType, std::shared_ptr<ButtonEffect>> effects;
	std::list<ButtonListener> listeners;
	std::list<std::shared_ptr<WindowEventListener>> addedWindowListeners;

	// TEMP
	std::list<Point> touchPoints;
	// TEMP
};

template<typename Func>
inline void Button::AddListener(Func&& func)
{
	listeners.emplace_back(std::forward<Func>(func));
}
*/
