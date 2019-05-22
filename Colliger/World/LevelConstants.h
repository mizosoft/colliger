#pragma once

#include <list>

class LevelConstants
{
private: 
	using DurationList = std::list<float>;
	using DurationIter = DurationList::iterator;
public:
	LevelConstants();
	~LevelConstants() = default;

	float GetFrightenedDuration();

	float GetChasingDuration();

	float GetScatteringDuration();
private:
	float Next(std::pair<DurationList, DurationIter>& pair);
public:
	static LevelConstants GetLevelConstans(int levelCount);
private:
	std::pair<DurationList, DurationIter> frightenedDurations;
	std::pair<DurationList, DurationIter> chasingDurations;
	std::pair<DurationList, DurationIter> scatteringDurations;
};
