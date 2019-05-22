#pragma once

#include <map>
#include <list>

#include "GhostStyle.h"
#include "Direction.h"

class GhostAnimationData
{
private:
	GhostAnimationData();
public:
	~GhostAnimationData() = default;

	std::list<int> GetFrameSequence(GhostStyle style, Direction direction) const;
public:
	static const float DEFAULT_FRAME_DURATION;
	static const GhostAnimationData INSTANCE;
private:
	std::map<GhostStyle, std::list<int>> fullFrameSequence;
	std::map<Direction, std::pair<int, int>> frameSequenceRanges;
};
