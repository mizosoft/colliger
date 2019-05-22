#pragma once

#include "SFML/Graphics/Sprite.hpp"
#include "Core/Drawable.h"
#include "Core/Updatable.h"

class Tile;

class GameWorld;

class Entity :
	public Drawable,
	public Updatable
{
public:
	Entity(const GameWorld& gameWorld);
	virtual ~Entity() = default;

	const GameWorld& GetGameWorld() const;

	void FixOriginOnCenter();

	sf::Vector2f GetCenterPosition() const;

	void SetCenterPosition(const sf::Vector2f& position);

	bool CollidesWith(const Entity& other) const;

	const Tile& GetEntityTile() const;

	virtual void InitializeSprite();

	virtual void Update(float deltaTime) override;
protected:
	virtual void Draw(sf::RenderTarget& target, sf::RenderStates states) const override;
protected:
	sf::Sprite sprite;
	const GameWorld& gameWorld;
};
