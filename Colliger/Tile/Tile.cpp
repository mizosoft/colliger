#include "Tile.h"

const Tile Tile::INVALID_TILE = Tile();

Tile::Tile() : 
	i(-1), j(-1),
	layerId(-1),
	token(Token::INVALID)
{
}

Tile::Tile(int layerId, int i, int j, const sf::FloatRect& tileRect) :
	layerId(layerId),
	i(i), j(j), 
	token(Token::EMPTY),
	tileRect(tileRect)
{
}

Tile::Tile(int layerId, int i, int j, const sf::Sprite& sprite) :
	layerId(layerId),
	i(i), j(j), 
	token(Token::MAZE_TILE), 
	sprite(sprite), 
	tileRect(sprite.getGlobalBounds())
{
}

int Tile::GetLayerId() const
{
	return layerId;
}

int Tile::GetRowIndex() const
{
	return i;
}

int Tile::GetColumnIndex() const
{
	return j;
}

Tile::Token Tile::GetToken() const
{
	return token;
}

const sf::FloatRect& Tile::GetTileRectangle() const
{
	return tileRect;
}

sf::Vector2f Tile::GetTileCenter() const
{
	return sf::Vector2f(tileRect.left + tileRect.width / 2.f, tileRect.top + tileRect.height / 2.f);
}

void Tile::Draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (token != Token::EMPTY)
	{
		target.draw(sprite, states);
	}
}

bool Tile::operator==(const Tile& other) const
{
	return layerId == other.layerId && i == other.i && j == other.j && token == other.token;
}
