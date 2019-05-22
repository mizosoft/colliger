#include "FadeAlpha.h"

#include "Helper/Utils/Math.h"

const float FadeAlpha::FULL_ALPHA = 255;

FadeAlpha::FadeAlpha(Type type) :
	type(type),
	running(true),
	alpha(),
	alphaIncreaseRate(0.f),
	minAlpha(0.f),
	maxAlpha(FULL_ALPHA)
{
	switch (type)
	{
	case FADE_IN:
		alpha = FULL_ALPHA;

		break;
	case FADE_OUT:
		alpha = 0.f;
	}
}

float FadeAlpha::GetAlpha() const
{
	return alpha;
}

bool FadeAlpha::IsRunning() const
{
	return running;
}

void FadeAlpha::SetAlphaRange(float minAlpha, float maxAlpha)
{
	this->minAlpha = Min(FULL_ALPHA, Max(0.f, minAlpha));
	this->maxAlpha = Min(FULL_ALPHA, Max(minAlpha, maxAlpha));
}

void FadeAlpha::SetAlphaIncreaseRate(float alphaIncreaseRate)
{
	this->alphaIncreaseRate = std::abs(alphaIncreaseRate);
}

void FadeAlpha::Update(float deltaTime)
{
	if (running)
	{
		switch (type)
		{
		case FADE_IN: // Alpha decreasing
			alpha = Max(minAlpha, alpha - deltaTime * alphaIncreaseRate);

			if (alpha <= minAlpha)
			{
				running = false;
			}

			break;
		case FADE_OUT: 
			alpha = Min(maxAlpha, alpha + deltaTime * alphaIncreaseRate);

			if (alpha >= maxAlpha)
			{
				running = false;
			}
		}
	}
}
