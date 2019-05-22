#include "Entity.h"
#include "World/GameWorld.h"
#include "Tile/Tile.h"

Entity::Entity(const GameWorld& gameWorld) :
	gameWorld(gameWorld)
{
}

const GameWorld& Entity::GetGameWorld() const
{
	return gameWorld;
}

void Entity::FixOriginOnCenter()
{
	sf::FloatRect bounds = sprite.getLocalBounds();
	this->sprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
}

sf::Vector2f Entity::GetCenterPosition() const
{
	return sprite.getPosition();
}

void Entity::SetCenterPosition(const sf::Vector2f & position)
{
	FixOriginOnCenter();
	sprite.setPosition(position);
}

bool Entity::CollidesWith(const Entity & other) const
{
	return GetEntityTile().GetTileRectangle() == other.GetEntityTile().GetTileRectangle();
}

const Tile& Entity::GetEntityTile() const
{
	return gameWorld.GetTileMap().GetTileAt(sprite.getPosition());
}

void Entity::InitializeSprite()
{
	// NO-OP
}

void Entity::Update(float deltaTime)
{
	// NO-OP
}

void Entity::Draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	target.draw(sprite, states);
}