#pragma once

#include "MoveableEntity.h"
#include "Helper/Resource/ResourceManager.h"
#include "Helper/Graphics/Animation.h"
#include "MovementListener.h"
#include "Helper/Event/WindowEventDispatcher.h"
#include "ColligerAnimationData.h"

class Colliger :
	public MoveableEntity
{
public:
	enum State
	{
		NORMAL,
		DEAD
	};

	Colliger(ResourceManager& manager, WindowEventDispatcher& dispatcher, const GameWorld& gameWorld);
	~Colliger() = default;

	ColligerOrientation GetOrientation() const;

	virtual void InitializeSprite() override;

	virtual void Update(float deltaTime) override;
private:
	static const float INITIAL_SPEED;
	static const float UPDATE_DIRECTION_OFFSET;
	static const float FRAME_DURATION;

	State state;
	Animation animation;
	ColligerAnimationData data;
	std::shared_ptr<MovementListener> listener;
};
