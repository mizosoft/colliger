#pragma once

#include <list>

#include "DelayedScene.h"
#include "Entity/Direction.h"
#include "GhostActor_1.h"
#include "ColligerActor_1.h"
#include "Helper/Utils/Random.h"

class ResourceManager;

class PursuitScene_1 :
	public DelayedScene
{
public:
	enum Type
	{
		DISABLED,
		COLLIGER_CHASING,
		COLLIGER_CHASED
	};
public:
	PursuitScene_1(const sf::Vector2u& worldBounds, ResourceManager& resourceManager);
	~PursuitScene_1() = default;

	void Init(
		Type type,
		Direction entranceDirection,
		float scaleY,
		int ghostCount
	);

	void SetActorVelocity(float actorVelocity);

	void SetActorSpacing(float actorSpacing);

	Type GetType() const;

	Direction GetEntranceDirection() const;

	virtual void Update(float deltaTime) override;

	virtual void Draw(sf::RenderTarget& target, sf::RenderStates states) const override;
private:
	std::list<GhostStyle> GenerateGhostStyles(Type type, int ghostCount) const;
private:
	Type type;
	ResourceManager& resourceManager;
	sf::Vector2u worldBounds;
	float actorVelocity, actorSpacing;
	ColligerActor_1 colliger;
	std::list<GhostActor_1> ghosts;
	mutable Random<int> random;
};
