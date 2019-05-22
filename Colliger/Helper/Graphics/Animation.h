#pragma once

#include <list>

#include "SFML/Graphics/Sprite.hpp"
#include "Tile/TileSet.h"
#include "Core/Updatable.h"
#include "Helper/Utils/Ticker.h"

class Animation :
	public Updatable
{
public:
	enum Type
	{
		LOOP,
		FIXED
	};
public:
	Animation(std::shared_ptr<TileSet> tileSet);
	~Animation() = default;

	void SetType(Type type);

	void SetFrameDuration(float frameDuration);

	void SetFrameSequence(const std::list<int>& frameSequence);

	void FillNext(sf::Sprite& sprite);

	void SetTexture(sf::Sprite& sprite);

	void Freeze();

	void FreezeOn(int frame);

	void Resume();

	void Restart();

	bool IsRunning() const;

	std::shared_ptr<TileSet> GetTileSet();

	virtual void Update(float deltaTime) override;
private:
	bool running;
	Ticker ticker;
	Type type;
	float frameDuration;
	std::list<int> frameSequence;
	std::list<int>::iterator currentFrame;
	std::shared_ptr<TileSet> tileSet;
};
