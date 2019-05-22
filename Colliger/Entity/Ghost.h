#pragma once

#include "MoveableEntity.h"
#include "GhostBehaviour.h"
#include "Helper/Graphics/Animation.h"
#include "GhostAnimationData.h"
#include "GhostState.h"

class ResourceManager;

class Ghost :
	public MoveableEntity
{
public:
	Ghost(GhostStyle style, GhostBehaviour *behaviour, ResourceManager& manager, const GameWorld& world);
	~Ghost();

	GhostState GetState() const;

	void SetState(GhostState state);

	GhostStyle GetStyle() const;

	virtual void Update(float deltaTime) override;

	virtual void Draw(sf::RenderTarget& target, sf::RenderStates states) const override;
private:
	void UpdateAnimation();
private:
	static const float INITIAL_SPEED;
	static const float UPDATE_DIRECTION_OFFSET;

	GhostState state;
	GhostStyle style;
	Route currentRoute;
	GhostBehaviour* behaviour; // Ptr cuz this is a base virtual type 
	Animation animation;
	GhostAnimationData data;
};
