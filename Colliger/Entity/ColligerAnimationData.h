#pragma once

#include <list>
#include <map>

#include "ColligerStyle.h"
#include "ColligerOrientation.h"
#include "ColligerMouthLevel.h"

class ColligerAnimationData
{
private:
	ColligerAnimationData();
public:
	~ColligerAnimationData() = default;

	std::list<int> GetFrameSequence(ColligerOrientation orientation, ColligerStyle style) const;

	int GetFrame(ColligerOrientation orientation, ColligerStyle style, ColligerMouthLevel mouthLevel) const;
public:
	static const float DEFAULT_FRAME_DURATION;
	static const ColligerAnimationData INSTANCE;
private:
	std::map<ColligerOrientation, std::list<int>> fullFrameSequence;
	std::map<ColligerStyle, std::pair<int, int>> frameSequenceRanges;
};
