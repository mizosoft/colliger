#pragma once

enum Direction
{
	RIGHT,
	UP,
	LEFT,
	DOWN,
	NO_MOVEMENT
};

enum DirectionAlignment
{
	ORTHOGONAL,
	ADJACENT
};

DirectionAlignment GetAlignment(Direction d1, Direction d2);
