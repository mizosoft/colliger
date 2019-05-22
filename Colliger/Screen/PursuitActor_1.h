#pragma once

#include "Core/Updatable.h"
#include "Core/Drawable.h"
#include "SFML/Graphics/Sprite.hpp"
#include "Helper/Graphics/Animation.h"
#include "Entity/Direction.h"

class PursuitActor_1 :
	public Updatable,
	public Drawable
{
public:
	PursuitActor_1(std::shared_ptr<TileSet> tileSet);
	virtual ~PursuitActor_1() = default;

	// Direction dependent
	float GetTail() const;

	sf::FloatRect GetBounds() const;

	Direction GetDirection() const;

	void FreezeAnimationOn(int freezeFrame);

	void SetDirection(Direction direction);

	void SetCenterPosition(const sf::Vector2f& position);

	sf::Vector2f GetCenterPosition() const;

	void SetScale(const sf::Vector2f& scale);

	void SetVelocity(float velocity);

	virtual void UpdateAnimation() = 0;

	virtual void Update(float deltaTime) override;

	virtual void Draw(sf::RenderTarget& target, sf::RenderStates states) const override;
protected:
	float velocity;
	Direction direction;
	sf::Sprite sprite;
	Animation animation;
};
