#pragma once

#include "Entity.h"
#include "Helper/Graphics/Animation.h" 

class ResourceManager;

class Coin :
	public Entity
{
public:
	Coin(ResourceManager& resourceManager, const GameWorld& world);
	~Coin() = default;

	virtual void InitializeSprite() override;

	virtual void Update(float deltaTime) override;
private:
	static const float FRAME_DURATION;
	static const int FRAME_SEQ_FROM;
	static const int FRAME_SEQ_TO;

	Animation animation;
};

