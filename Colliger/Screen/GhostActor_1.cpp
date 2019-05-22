#include "GhostActor_1.h"

#include "Assets.h"
#include "Entity/GhostAnimationData.h"

GhostActor_1::GhostActor_1(ResourceManager& resourceManager) :
	PursuitActor_1(Assets::GHOST_TILE_SET.Get(resourceManager)), 
	style(GhostStyle::RED)
{
	animation.SetFrameDuration(GhostAnimationData::DEFAULT_FRAME_DURATION);
}

GhostStyle GhostActor_1::GetStyle() const
{
	return style;
}

void GhostActor_1::SetStyle(GhostStyle style)
{
	this->style = style;
}

void GhostActor_1::UpdateAnimation()
{
	const GhostAnimationData& data = GhostAnimationData::INSTANCE;
	animation.SetFrameSequence(data.GetFrameSequence(style, direction));
}
