#include "Ticker.h"

Ticker::Ticker() :
	elapsed(0.f)
{
}

void Ticker::Reset()
{
	elapsed = 0.f;
}

float Ticker::GetElapsed()
{
	return elapsed;
}

void Ticker::Update(float deltaTime)
{
	elapsed += deltaTime;
}
