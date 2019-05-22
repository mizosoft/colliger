#pragma once

#include "Core/Updatable.h"

class FadeAlpha :
	public Updatable
{
public:
	enum Type
	{
		FADE_IN,
		FADE_OUT
	};
public:
	FadeAlpha(Type type);
	~FadeAlpha() = default;

	float GetAlpha() const;

	bool IsRunning() const;

	void SetAlphaRange(float minAlpha, float maxAlpha);

	void SetAlphaIncreaseRate(float alphaIncreaseRate);

	virtual void Update(float deltaTime) override;
private:
	static const float FULL_ALPHA;

	Type type;
	bool running;
	float alpha;
	float alphaIncreaseRate;
	float minAlpha, maxAlpha;
};
