#include "Colliger.h"

#include "World/GameWorld.h"
#include "Helper/Event/EventDispatcher.h"

const float Colliger::INITIAL_SPEED = 180.f;
const float Colliger::UPDATE_DIRECTION_OFFSET = 2.f;
const float Colliger::FRAME_DURATION = 0.085f;

Colliger::Colliger(ResourceManager& manager, WindowEventDispatcher& dispatcher, const GameWorld& gameWorld) :
	MoveableEntity(gameWorld),
	state(State::NORMAL),
	data(ColligerAnimationData::INSTANCE),
	listener(dispatcher.Subscribe(sf::Event::KeyPressed, MovementListener())),
	animation(manager.GetResource<TileSet>("ColligerTileSet.json"))
{
	animation.SetFrameSequence(
		data.GetFrameSequence(ColligerOrientation::LOOKING_RIGHT, ColligerStyle::NORMAL)
	);
	animation.SetFrameDuration(FRAME_DURATION);

	speed = INITIAL_SPEED;
	updateDirectionOffset = UPDATE_DIRECTION_OFFSET;

	sprite.setTexture(*animation.GetTileSet()->GetTexture());
	animation.FillNext(sprite);
	FixOriginOnCenter();
}

ColligerOrientation Colliger::GetOrientation() const
{
	switch (direction)
	{
	case LEFT:
		return ColligerOrientation::LOOKING_LEFT;
	case RIGHT:
		return ColligerOrientation::LOOKING_RIGHT;
	case UP:
		return ColligerOrientation::LOOKING_BACK;
	default: // Down assumed
		return ColligerOrientation::LOOKING_FRONT;
	}
}

void Colliger::InitializeSprite()
{
}

void Colliger::Update(float deltaTime)
{
	Direction next = listener->GetCurrentDirection();
	if (IsReadyToUpdateDirection(next))
	{
		Tile::Token nextTileToken = Tile::Token::INVALID;
		const Tile& entityTile = GetEntityTile();
		const TileMap& tileMap = gameWorld.GetTileMap();
		switch (next)
		{
		case LEFT:
			nextTileToken = tileMap.GetLeftTile(entityTile).GetToken();

			break;
		case RIGHT:
			nextTileToken = tileMap.GetRightTile(entityTile).GetToken();

			break;
		case UP:
			nextTileToken = tileMap.GetUpperTile(entityTile).GetToken();

			break;
		case DOWN:
			nextTileToken = tileMap.GetLowerTile(entityTile).GetToken();
		}

		if (nextTileToken == Tile::EMPTY && next != direction)
		{
			direction = next;
			if (direction == Direction::RIGHT || direction == Direction::LEFT)
			{
				animation.SetFrameSequence(data.GetFrameSequence(GetOrientation(), ColligerStyle::NORMAL));
			}

			// Resume animation if previously frozen
			animation.Resume();
		}
	}

	Move(deltaTime);
	
	if (direction == NO_MOVEMENT && animation.IsRunning())
	{
		animation.FreezeOn(data.GetFrame(GetOrientation(), ColligerStyle::NORMAL, ColligerMouthLevel::OPEN_3));
	}

	animation.Update(deltaTime);
	animation.FillNext(sprite);
}
