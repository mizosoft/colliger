#pragma once

#include "Core/Updatable.h"

class Ticker :
	public Updatable
{ 
public:
	Ticker();
	~Ticker() = default;

	void Reset();

	float GetElapsed();

	virtual void Update(float deltaTime) override;
private:
	float elapsed;
};

