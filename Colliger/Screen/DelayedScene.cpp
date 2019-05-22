#include "DelayedScene.h"

DelayedScene::DelayedScene() :
	state(PlayState::PRE_PLAY),
	delayInSeconds(0.f),
	sceneTicker()
{
}

void DelayedScene::SetDelay(float delayInSeconds)
{
	this->delayInSeconds = delayInSeconds;
}

DelayedScene::PlayState DelayedScene::GetState() const
{
	return state;
}

void DelayedScene::UpdateTicker(float deltaTime)
{
	if (state == PlayState::PRE_PLAY)
	{
		sceneTicker.Update(deltaTime);
		if (sceneTicker.GetElapsed() >= delayInSeconds)
		{
			state = PlayState::PLAYING;
		}
	}
}
