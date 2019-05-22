#pragma once

#include "GhostBehaviour.h"

class AmbushGhostBehaviour :
	public GhostBehaviour
{
public:
	AmbushGhostBehaviour() = default;
	~AmbushGhostBehaviour() = default;

	virtual Route GetNextRoute(const Ghost& ghost) override;
};

