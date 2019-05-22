#pragma once

#include <memory>

#include "SFML/Graphics/Sprite.hpp"
#include "SFML/System/Vector2.hpp"
#include "SFML/Graphics/Rect.hpp"
#include "Core/Drawable.h"

class Tile : 
	public Drawable
{
	friend class TileLayer;
public:
	enum Token
	{
		EMPTY,
		MAZE_TILE,
		INVALID
	};
private:
	Tile(); // Creates an INVALID Tile
	Tile(int layerId, int i, int j, const sf::Sprite& sprite); // Creates a MAZE_TILE Tile
	Tile(int layerId, int i, int j, const sf::FloatRect& pos); // Creates an EMPTY Tile
public:
	~Tile() = default;

	int GetLayerId() const;

	int GetRowIndex() const;

	int GetColumnIndex() const;

	Token GetToken() const;

	const sf::FloatRect& GetTileRectangle() const;

	sf::Vector2f GetTileCenter() const;

	virtual void Draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	bool operator==(const Tile& other) const;
public:
	static const Tile INVALID_TILE;
private:
	int layerId;
	int i, j;
	Token token;
	sf::Sprite sprite; // The sprite representing the tile's texture region and position, empty if Token::Empty or Token::Invalid
	sf::FloatRect tileRect; // The tile's bounds, empty if Token::INVALID
};
