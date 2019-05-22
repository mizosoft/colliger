#include "GhostAnimationData.h"

const float GhostAnimationData::DEFAULT_FRAME_DURATION = 0.085f;
const GhostAnimationData GhostAnimationData::INSTANCE = GhostAnimationData();

GhostAnimationData::GhostAnimationData() :
	fullFrameSequence(),
	frameSequenceRanges()

{
	std::list<GhostStyle> styles =
	{
		GhostStyle::BLUE,
		GhostStyle::RED,
		GhostStyle::PINK,
		GhostStyle::GREEN,
		GhostStyle::YELLO,
		GhostStyle::CYAN,
		GhostStyle::FRIGHTENED_LOOK,
		GhostStyle::NAKED
	};
	int counter = 1;
	const int framesPerRow = 12;
	for (GhostStyle style : styles)
	{
		std::list<int>& lst = fullFrameSequence[style];
		int s = counter;
		for (; counter < s + framesPerRow; counter++)
		{
			lst.push_back(counter);
		}
	}

	std::list<Direction> directions =
	{
		Direction::LEFT,
		Direction::RIGHT,
		Direction::DOWN,
		Direction::UP,
	};
	counter = 0; // Recycling variables be like
	const int framesPerDirection = 3;
	for (Direction dir : directions)
	{
		frameSequenceRanges[dir] = { counter, counter + framesPerDirection };
		counter += framesPerDirection;
	}
}

std::list<int> GhostAnimationData::GetFrameSequence(GhostStyle style, Direction direction) const
{
	auto fullSeqIter = fullFrameSequence.find(style);
	auto seqRangeIter = frameSequenceRanges.find(direction);
	if (fullSeqIter != fullFrameSequence.end() && seqRangeIter != frameSequenceRanges.end())
	{
		const std::list<int>& fullSeq = fullSeqIter->second;
		const std::pair<int, int>& seqRange = seqRangeIter->second;
		auto begin = fullSeq.begin(), end = fullSeq.begin();
		std::advance(begin, seqRange.first);
		std::advance(end, seqRange.second);

		return std::list<int>(begin, end);
	}

	return std::list<int>();
}
