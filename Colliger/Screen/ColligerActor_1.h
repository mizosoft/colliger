#pragma once

#include "PursuitActor_1.h"
#include "Entity/ColligerAnimationData.h"

class ResourceManager;

class ColligerActor_1 :
	public PursuitActor_1
{
public:
	ColligerActor_1(ResourceManager& resourceManager);
	~ColligerActor_1() = default;

	ColligerOrientation GetOrientation() const;

	ColligerStyle GetStyle() const;

	void SetStyle(ColligerStyle style);

	virtual void UpdateAnimation() override;
private:
	ColligerStyle style;
};
