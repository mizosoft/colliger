#include <exception>

#include "Animation.h"

Animation::Animation(std::shared_ptr<TileSet> tileSet) :
	running(false),
	ticker(),
	type(Type::LOOP),
	frameDuration(0.f),
	frameSequence(),
	currentFrame(frameSequence.end()),
	tileSet(tileSet)
{
}

void Animation::SetType(Type type)
{
	this->type = type;
}

void Animation::SetFrameDuration(float frameDuration)
{
	this->frameDuration = frameDuration;
}

void Animation::SetFrameSequence(const std::list<int>& frameSequence)
{
	for (int frame : frameSequence)
	{
		if (tileSet->FindRegion(frame) == TileSet::REGION_NOT_FOUND)
		{
			throw std::invalid_argument("Invalid frame: " + std::to_string(frame));
		}
	}

	this->frameSequence = std::move(frameSequence);
	currentFrame = this->frameSequence.begin();
	running = true;
	ticker.Reset();
}

void Animation::FillNext(sf::Sprite& sprite)
{
	float elapsed = ticker.GetElapsed();
	if (running && elapsed >= frameDuration)
	{
		if (currentFrame != frameSequence.end())
		{
			currentFrame++;
		}

		switch (type)
		{
		case LOOP:
			if (currentFrame == frameSequence.end())
			{
				currentFrame = frameSequence.begin();
			}

			break;
		case FIXED:
			if (currentFrame == frameSequence.end() &&
				currentFrame != frameSequence.begin())
			{
				currentFrame--; // Set currentFrame back to the last frame
			}

			break;
		default: {}
		}

		ticker.Reset();
	}

	if (currentFrame != frameSequence.end())
	{
		sprite.setTextureRect(tileSet->FindRegion(*currentFrame));
	}
}

void Animation::SetTexture(sf::Sprite& sprite)
{
	sprite.setTexture(*tileSet->GetTexture());
}

void Animation::Freeze()
{
	running = false;
}

void Animation::FreezeOn(int frame)
{
	std::list<int>::iterator iter = std::find(frameSequence.begin(), frameSequence.end(), frame);
	if (iter != frameSequence.end())
	{
		currentFrame = iter;
		running = false;
		ticker.Reset();
	}
}

void Animation::Resume()
{
	running = true;
}

void Animation::Restart()
{
	currentFrame = frameSequence.begin();
}

bool Animation::IsRunning() const
{
	return running;
}

std::shared_ptr<TileSet> Animation::GetTileSet()
{
	return tileSet;
}

void Animation::Update(float deltaTime)
{
	if (running)
	{
		ticker.Update(deltaTime);
	}
}
