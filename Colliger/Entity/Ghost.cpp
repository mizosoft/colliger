#include "Ghost.h"

#include "Assets.h"

const float Ghost::INITIAL_SPEED = 180.f;
const float Ghost::UPDATE_DIRECTION_OFFSET = 2.f;

Ghost::Ghost(GhostStyle style, GhostBehaviour* behaviour, ResourceManager& manager, const GameWorld& world) :
	MoveableEntity(world),
	state(IDLE),
	style(style),
	currentRoute({}),
	behaviour(behaviour),
	animation(manager.GetResource<TileSet>("GhostTileSet.json")),
	data(GhostAnimationData::INSTANCE)
{
	direction = Direction::RIGHT;
	animation.SetFrameSequence(
		data.GetFrameSequence(style, direction)
	);
	animation.SetFrameDuration(GhostAnimationData::DEFAULT_FRAME_DURATION);

	speed = INITIAL_SPEED;
	updateDirectionOffset = UPDATE_DIRECTION_OFFSET;

	sprite.setTexture(*animation.GetTileSet()->GetTexture());
	animation.FillNext(sprite);
	FixOriginOnCenter();
}

Ghost::~Ghost()
{
	if (behaviour)
	{
		delete behaviour;
	}
}

GhostState Ghost::GetState() const
{
	return state;
}

void Ghost::SetState(GhostState state)
{
	this->state = state;

	UpdateAnimation();
}

GhostStyle Ghost::GetStyle() const
{
	return style;
}

void Ghost::Update(float deltaTime)
{	
	if (!currentRoute.HasNext())
	{
		currentRoute = behaviour->GetNextRoute(*this);
	}

	Direction next = Direction::NO_MOVEMENT;
	if (currentRoute.HasNext())
	{
		next = currentRoute.GetNext(*this);
	}
		
	if (IsReadyToUpdateDirection(next) && 
		next != direction && 
		next != Direction::NO_MOVEMENT)
	{
		std::cout << std::endl << "next direction: " << next << std::endl;
		direction = next;

		UpdateAnimation();
	}

	Move(deltaTime);

	animation.Update(deltaTime);
	animation.FillNext(sprite);
}

void Ghost::Draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(sprite, states);
	target.draw(currentRoute, states);
}

void Ghost::UpdateAnimation()
{
	animation.SetFrameSequence(data.GetFrameSequence(style, direction));
}
