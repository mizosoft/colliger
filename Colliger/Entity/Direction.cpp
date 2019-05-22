#include "Direction.h"

#include <cmath>

DirectionAlignment GetAlignment(Direction d1, Direction d2) 
{
	return abs(d1 - d2) % 2 == 0 || (d1 | d2 & NO_MOVEMENT) == NO_MOVEMENT ? ADJACENT : ORTHOGONAL;
}