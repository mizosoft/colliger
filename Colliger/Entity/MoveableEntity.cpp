#include "MoveableEntity.h"
#include "World/GameWorld.h"

MoveableEntity::MoveableEntity(const GameWorld& gameWorld) :
	Entity(gameWorld),
	direction(Direction::NO_MOVEMENT),
	speed(0.f),
	updateDirectionOffset(0.f)
{
}

Direction MoveableEntity::GetDirection() const
{
	return direction;
}

void MoveableEntity::Move(float deltaTime)
{
	const Tile& entityTile = GetEntityTile();
	const TileMap& tileMap = gameWorld.GetTileMap();
	sf::Vector2f position(sprite.getPosition()),
		tileCenter(entityTile.GetTileCenter());
	float inc = speed * deltaTime;
	switch (direction)
	{
	case LEFT:
	{
		const Tile& left = tileMap.GetLeftTile(entityTile);
		if (left.GetToken() == Tile::MAZE_TILE)
		{
			if (position.x <= tileCenter.x)
			{
				// Reached a dead end, we stop
				direction = Direction::NO_MOVEMENT;
				// position.x = tileCenter.x;
			}

			// if The next token isn't empty and we preserve 
			// the direction, we should make sure that the tileCenter
			// won't be exceeded
			position.x = std::max(tileCenter.x, position.x - inc);
		}
		else if (left.GetToken() == Tile::EMPTY)
		{
			position.x -= inc;
		}

		break;
	}
	case RIGHT:
	{
		const Tile& right = tileMap.GetRightTile(entityTile);
		if (right.GetToken() == Tile::MAZE_TILE)
		{
			if (position.x >= tileCenter.x)
			{
				// Reached a dead end, we stop
				direction = Direction::NO_MOVEMENT;
				// position.x = tileCenter.x;
			}

			// if The next token isn't empty and we preserve 
			// the direction, we should make sure that the tileCenter
			// won't be exceeded
			position.x = std::min(tileCenter.x, position.x + inc);
		}
		else if (right.GetToken() == Tile::EMPTY)
		{
			position.x += inc;
		}

		break;
	}
	case UP:
	{
		const Tile& upper = tileMap.GetUpperTile(entityTile);
		if (upper.GetToken() == Tile::MAZE_TILE)
		{
			if (position.y <= tileCenter.y)
			{
				// Reached a dead end, we stop
				direction = Direction::NO_MOVEMENT;
				// position.x = tileCenter.x;
			}

			// if The next token isn't empty and we preserve 
			// the direction, we should make sure that the tileCenter
			// won't be exceeded
			position.y = std::max(tileCenter.y, position.y - inc);
		}
		else if (upper.GetToken() == Tile::EMPTY)
		{
			position.y -= inc;
		}

		break;
	}
	case DOWN:
	{
		const Tile& lower = tileMap.GetLowerTile(entityTile);
		if (lower.GetToken() == Tile::MAZE_TILE)
		{
			if (position.y >= tileCenter.y)
			{
				// Reached a dead end, we stop
				direction = Direction::NO_MOVEMENT;
				// position.x = tileCenter.x;
			}

			// if The next token isn't empty and we preserve 
			// the direction, we should make sure that the tileCenter
			// won't be exceeded
			position.y = std::min(tileCenter.y, position.y + inc);
		}
		else if (lower.GetToken() == Tile::EMPTY)
		{
			position.y += inc;
		}

		break;
	}
	}

	sprite.setPosition(position);
}

bool MoveableEntity::IsReadyToUpdateDirection(Direction next)
{
	const sf::Vector2f& pos = sprite.getPosition();
	sf::Vector2f tileCenter = GetEntityTile().GetTileCenter();
	switch (GetAlignment(direction, next))
	{
		// Left-Right, Up-Down direction switches 
		// should be available any time
	case ADJACENT:
		return true;
		// If ORTHOGONAL, we need to wait till we 
		// reach the tiles center (doesn't check
		// if the orthogonal tile is EMPTY or not
		// it just checks if we are ready to make 
		// an orthogonal direction switch)
	case ORTHOGONAL:
		switch (direction)
		{
		case LEFT: case RIGHT:
			return abs(pos.x - tileCenter.x) <= updateDirectionOffset;
		case UP: case DOWN:
			return abs(pos.y - tileCenter.y) <= updateDirectionOffset;
		case NO_MOVEMENT:  // Impossible ? 
			return true;
		}
	default: // IMPOSSIBLE ?????
		return true;
	}
}
