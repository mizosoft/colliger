#include "Coin.h"
#include "Assets.h"
#include "Helper/Resource/ResourceManager.h"

const float Coin::FRAME_DURATION = 0.15f;
const int Coin::FRAME_SEQ_FROM = 10;
const int Coin::FRAME_SEQ_TO = 10;

Coin::Coin(ResourceManager& resourceManager, const GameWorld& world) :
	Entity(world),
	animation(Assets::FOOD_TILE_SET.Get(resourceManager))
{
	std::list<int> list;
	for (int i = FRAME_SEQ_FROM; i <= FRAME_SEQ_TO; i++)
	{
		list.push_back(i);
	}
	animation.SetFrameSequence(list);
	animation.SetFrameDuration(FRAME_DURATION);
}

void Coin::InitializeSprite()
{
	sprite.setTexture(*animation.GetTileSet()->GetTexture());
	animation.FillNext(sprite);
}

void Coin::Update(float deltaTime)
{
	animation.Update(deltaTime);
	animation.FillNext(sprite);
}
