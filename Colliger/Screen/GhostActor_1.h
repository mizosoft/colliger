#pragma once

#include "PursuitActor_1.h"
#include "Entity/GhostAnimationData.h"

class ResourceManager;

class GhostActor_1 :
	public PursuitActor_1
{
public:
	GhostActor_1(ResourceManager& resourceManager);
	~GhostActor_1() = default;

	GhostStyle GetStyle() const;

	void SetStyle(GhostStyle style);

	virtual void UpdateAnimation() override;
private:
	GhostStyle style;
};
