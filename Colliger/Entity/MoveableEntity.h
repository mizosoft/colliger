#pragma once

#include "Entity.h"
#include "Direction.h"

class MoveableEntity :
	public Entity
{
public:
	MoveableEntity(const GameWorld& gameWorld);
	virtual ~MoveableEntity() = default;
public:
	Direction GetDirection() const;
protected:
	void Move(float deltaTime);

	bool IsReadyToUpdateDirection(Direction next);
protected:
	Direction direction;
	float speed;
	// How many pixels away from the tile center
	// should we be able to update the direction
	// if orthogonaly aligned
	float updateDirectionOffset;
};