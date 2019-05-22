#include "LevelConstants.h"



LevelConstants::LevelConstants()
{
}

float LevelConstants::GetFrightenedDuration()
{
	return Next(frightenedDurations);
}

float LevelConstants::GetChasingDuration()
{
	return Next(chasingDurations);
}

float LevelConstants::GetScatteringDuration()
{
	return Next(scatteringDurations);
}

float LevelConstants::Next(std::pair<DurationList, DurationIter>& pair)
{
	DurationList list = pair.first;
	DurationIter& iter = pair.second;
	if (iter == list.end())
	{
		return *list.rbegin();
	}
	else
	{
		iter++;

		return *iter;
	}
}

LevelConstants LevelConstants::GetLevelConstans(int levelCount)
{
	// This should be saved in a file or something
	// but currently we have 1 level

	// TODO 
	return LevelConstants();
}
