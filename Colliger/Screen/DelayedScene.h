#pragma once

#include "Core/Updatable.h"
#include "Core/Drawable.h"
#include "Helper/Utils/Ticker.h"

class DelayedScene :
	public Updatable,
	public Drawable
{
public:
	enum PlayState
	{
		PRE_PLAY,
		PLAYING,
		FINISHED
	};
public:
	DelayedScene();
	virtual ~DelayedScene() = default;

	void SetDelay(float delayInSeconds);

	PlayState GetState() const;

	void UpdateTicker(float deltaTime);
protected:
	PlayState state;
	float delayInSeconds;
	Ticker sceneTicker;
};
