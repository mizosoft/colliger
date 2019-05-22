#include "ColligerAnimationData.h"

#include <iostream>

const float ColligerAnimationData::DEFAULT_FRAME_DURATION = 0.09f;
const ColligerAnimationData ColligerAnimationData::INSTANCE = ColligerAnimationData();

ColligerAnimationData::ColligerAnimationData()
{
	std::list<ColligerOrientation> orientations =
	{
		ColligerOrientation::LOOKING_LEFT,
		ColligerOrientation::LOOKING_RIGHT,
		ColligerOrientation::LOOKING_FRONT
	};
	int counter = 1;
	const int framesPerRow = 8;
	for (ColligerOrientation orientation : orientations)
	{
		std::list<int>& lst = fullFrameSequence[orientation];
		for (int s = counter; counter < s + framesPerRow; counter++)
		{
			lst.push_back(counter);
		}
	}
	fullFrameSequence[ColligerOrientation::LOOKING_BACK].push_back(counter); // Back has only one frame
	
	std::list<ColligerStyle> styles =
	{
		ColligerStyle::RED_EYED,
		ColligerStyle::NORMAL
	}; // Weird flex but OK
	counter = 0;
	const int framesPerStyle = 4;
	for (ColligerStyle style : styles)
	{
		frameSequenceRanges[style] = { counter, counter + framesPerStyle };
		counter += framesPerStyle;
	}
}

std::list<int> ColligerAnimationData::GetFrameSequence(ColligerOrientation orientation, ColligerStyle style) const
{
	switch (orientation)
	{
	case LOOKING_BACK:
		return fullFrameSequence.at(orientation);
	default:
	{
		const std::list<int>& fullSeq = fullFrameSequence.at(orientation);
		const std::pair<int, int>& seqRange = frameSequenceRanges.at(style);
		auto begin = fullSeq.begin(), end = fullSeq.begin();
		std::advance(begin, seqRange.first);
		std::advance(end, seqRange.second);

		return std::list<int>(begin, end);
	}
	}
}

int ColligerAnimationData::GetFrame(ColligerOrientation orientation, ColligerStyle style, ColligerMouthLevel mouthLevel) const
{
	std::list<int> frameSeq = GetFrameSequence(orientation, style);
	int adv = 0;
	switch (orientation)
	{
	case LOOKING_RIGHT:
		adv = ColligerMouthLevel::OPEN_3 - mouthLevel;

		break;
	case LOOKING_LEFT:
	case LOOKING_FRONT:
		adv = mouthLevel;

		break;
	default:
		adv = 0;
	}

	auto iter = frameSeq.begin();
	std::advance(iter, adv);

	return *iter;
}
