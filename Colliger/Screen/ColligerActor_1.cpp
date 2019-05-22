#include "ColligerActor_1.h"

#include "Entity/ColligerAnimationData.h"
#include "Assets.h"

ColligerActor_1::ColligerActor_1(ResourceManager& resourceManager) :
	PursuitActor_1(Assets::COLLIGER_TILESET.Get(resourceManager)),
	style(ColligerStyle::NORMAL)
{
	animation.SetFrameDuration(ColligerAnimationData::DEFAULT_FRAME_DURATION);
}

ColligerOrientation ColligerActor_1::GetOrientation() const
{
	switch (direction)
	{
	case LEFT:
		return ColligerOrientation::LOOKING_LEFT;
	case RIGHT:
		return ColligerOrientation::LOOKING_RIGHT;
	case UP:
		return ColligerOrientation::LOOKING_BACK;
	default: // Down assumed
		return ColligerOrientation::LOOKING_FRONT;
	}
}

ColligerStyle ColligerActor_1::GetStyle() const
{
	return style;
}

void ColligerActor_1::SetStyle(ColligerStyle style)
{
	this->style = style;
}

void ColligerActor_1::UpdateAnimation()
{
	animation.SetFrameSequence(ColligerAnimationData::INSTANCE.GetFrameSequence(GetOrientation(), GetStyle()));
}
