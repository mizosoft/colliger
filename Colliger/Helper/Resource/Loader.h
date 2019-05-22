#pragma once

#include <memory>
#include <exception>

#include "SFML/Graphics/Texture.hpp"
#include "Tile/TileSet.h"
#include "Tile/TileMap.h"
#include "PathResolver.h"

class ResourceManager;

class LoadException :
	public std::exception
{
public:
	LoadException(const std::string& fileName, const std::string& reason = "", 
		std::exception_ptr cause = std::exception_ptr()) noexcept;
	~LoadException() = default;

	const std::string& GetFilName() const noexcept;

	const std::string& GetReason() const noexcept;

	std::string GetMessage() const noexcept;

	std::exception_ptr GetCause() const noexcept;
private:
	static std::string GetMessage(const LoadException& cause);

	std::string fileName;
	std::string reason;
	std::exception_ptr cause;
};

template<typename T>
struct Loader
{
};

template<>
struct Loader<sf::Font>
{
	inline std::shared_ptr<sf::Font> operator()(const std::string& fileName, std::shared_ptr<PathResolver> resolver) const
	{
		std::shared_ptr<sf::Font> ptr = std::make_shared<sf::Font>();
		if (!ptr->loadFromFile(fileName))
		{
			throw LoadException(fileName, "Font::loadFromFile returned false");
		}

		return ptr;
	}
};

template<>
struct Loader<sf::Texture>
{	
	inline std::shared_ptr<sf::Texture> operator()(const std::string& fileName, std::shared_ptr<PathResolver> resolver) const
	{
		std::shared_ptr<sf::Texture> ptr = std::make_shared<sf::Texture>();
		if (!ptr->loadFromFile(fileName))
		{
			throw LoadException(fileName, "Texture::loadFromFile returned false");
		}

		return ptr;
	}
};

template<>
struct Loader<TileSet>
{
	inline std::shared_ptr<TileSet> operator()(const std::string& fileName, std::shared_ptr<PathResolver> resolver) const
	{
		std::shared_ptr<TileSet> ptr;
		try
		{
			ptr = std::make_shared<TileSet>(fileName, resolver);
		}
		catch (...)
		{
			throw LoadException(fileName, "TileSet's contructor threw an exception", std::current_exception());
		}

		return ptr;
	}
};

template<>
struct Loader<TileMap>
{
	inline std::shared_ptr<TileMap> operator()(const std::string& fileName, std::shared_ptr<PathResolver> resolver) const
	{
		std::shared_ptr<TileMap> ptr;
		try
		{
			ptr = std::make_shared<TileMap>(fileName, resolver);
		}
		catch (...)
		{
			throw LoadException(fileName, "TileMap's contructor threw an exception", std::current_exception());
		}

		return ptr;
	}
};
