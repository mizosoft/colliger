#pragma once

#include <random>
#include <functional>
#include <type_traits>

// THIS SFINA TEMPLATE SHIT ROCKS!!!!

template <typename T, typename SFINAE = void>
struct DefaultDistributionOf;

template <typename T>
struct DefaultDistributionOf<T, typename std::enable_if<std::is_integral<T>::value>::type>
{
	using type = std::uniform_int_distribution<T>;
};

template <typename T>
struct DefaultDistributionOf<T, typename std::enable_if<std::is_floating_point<T>::value>::type>
{
	using type = std::uniform_real_distribution<T>;
};

template<typename T,
	typename Distribution = typename DefaultDistributionOf<T>::type,
	typename Engine = std::default_random_engine>
	class Random
{
public:
	Random(const Distribution& defaultDist = Distribution());
	~Random() = default;

	T Next();

	template<typename... Args>
	T Next(Args&& ... args);
private:
	Distribution defaultDist;
	Engine engine;
};

template<typename T, typename Distribution, typename Engine>
inline Random<T, Distribution, Engine>::Random(const Distribution& defaultDist) :
	defaultDist(defaultDist),
	engine(std::random_device()()) // Seed the engine with entropy
{
}

template<typename T, typename Distribution, typename Engine>
inline T Random<T, Distribution, Engine>::Next()
{
	return defaultDist(engine);
}

template<typename T, typename Distribution, typename Engine>
template<typename ...Args>
inline T Random<T, Distribution, Engine>::Next(Args&& ...args)
{
	Distribution _dist(std::forward<Args>(args)...);
	return _dist(engine);
}

// Well, i take back what i said about SFINAE and templates 
// cuz i took hours trying to make this useless shit work
