#include "PursuitActor_1.h"

PursuitActor_1::PursuitActor_1(std::shared_ptr<TileSet> tileSet) :
	velocity(0.f),
	direction(Direction::NO_MOVEMENT),
	sprite(*tileSet->GetTexture()),
	animation(tileSet)
{
}

float PursuitActor_1::GetTail() const
{
	sf::FloatRect bounds = GetBounds();
	switch (direction)
	{
	case LEFT:
		return bounds.left + bounds.width;
	case RIGHT:
		return bounds.left;
	case UP:
		return bounds.top + bounds.height;
	case DOWN:
		return bounds.top;
	default:
		return 0.f;
	}
}

sf::FloatRect PursuitActor_1::GetBounds() const
{
	return sprite.getGlobalBounds(); // TextureRect and position must be set
}

Direction PursuitActor_1::GetDirection() const
{
	return direction;
}

void PursuitActor_1::FreezeAnimationOn(int freezeFrame)
{
	animation.FreezeOn(freezeFrame);
	animation.FillNext(sprite);
}

void PursuitActor_1::SetDirection(Direction direction)
{
	this->direction = direction;

	// Set correct bounds
	UpdateAnimation();
	animation.FillNext(sprite);

	// Set origin to center
	sf::FloatRect bounds = GetBounds();
	sprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
}

void PursuitActor_1::SetCenterPosition(const sf::Vector2f& position)
{
	sprite.setPosition(position);
}

sf::Vector2f PursuitActor_1::GetCenterPosition() const
{
	return sprite.getPosition();
}

void PursuitActor_1::SetScale(const sf::Vector2f& scale)
{
	sprite.setScale(scale);
}

void PursuitActor_1::SetVelocity(float velocity)
{
	this->velocity = velocity;
}

void PursuitActor_1::Update(float deltaTime)
{
	animation.Update(deltaTime);
	animation.FillNext(sprite);

	sf::Vector2f movement;
	switch (direction)
	{
	case LEFT:
		movement.x -= velocity * deltaTime;

		break;
	case RIGHT:
		movement.x += velocity * deltaTime;

		break;
	case UP:
		movement.y -= velocity * deltaTime;

		break;
	case DOWN:
		movement.y += velocity * deltaTime;

		break;
	default: {}
	}

	sprite.move(movement);
}

void PursuitActor_1::Draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(sprite, states);
}
