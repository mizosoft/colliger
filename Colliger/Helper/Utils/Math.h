#pragma once

#include <cmath>

#include "SFML/System/Vector2.hpp"

constexpr long double PI = 3.14159265358979323846L;

template<typename T>
inline T Min(const T& t1, const T& t2)
{
	return t1 < t2 ? t1 : t2;
}

template<typename T>
inline T Max(const T& t1, const T& t2)
{
	return t1 > t2 ? t1 : t2;
}

template<typename T>
inline float GetDistance(const sf::Vector2<T>& p1, const sf::Vector2<T>& p2)
{
	T xDiff = p1.x - p2.x, yDiff = p1.y - p2.y;
	return static_cast<float>(std::sqrt(xDiff * xDiff + yDiff * yDiff));
}

template<typename T>
inline float GetAngleDegrees(const sf::Vector2<T>& v1, const sf::Vector2<T>& v2)
{
	float cos = (v1.x * v2.x + v1.y * v2.y) /
		(std::sqrt(v1.x * v1.x + v1.y * v1.y) * std::sqrt(v2.x * v2.x + v2.y * v2.y));
	return static_cast<float>(std::acos(cos) * (180 / PI));
}

/*template<typename T1, typename T2>
inline sf::Vector2<T2> CaseVector(const sf::Vector2<T1> v)
{
	return sf::Vector2<T2>(static_cast<T2>(v.x), static_cast<T2>(v.y));
}*/
