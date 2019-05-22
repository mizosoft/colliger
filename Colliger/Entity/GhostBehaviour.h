#pragma once

#include "Route.h"

class Ghost;

class GhostBehaviour
{
public:
	virtual ~GhostBehaviour() = default;
	
	virtual Route GetNextRoute(const Ghost& ghost) = 0;
};
