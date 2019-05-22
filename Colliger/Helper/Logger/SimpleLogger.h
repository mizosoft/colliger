#pragma once

#include <string>
#include <iostream>
#include <mutex>

/////////////////////////////////////////////////////////////////////////////////////

//TODO: revisit

class LogChain;

class SimpleLogger
{	
public:
	SimpleLogger(std::string name);

	enum Level
	{
		OFF,
		DEBUG,
		INFO,
		WARN,
		ERROR
	};

	LogChain operator<<(Level logLevel) const;
public:
	static bool initial;
	static const Level GLOBAL_LEVEL;
private:
	static std::recursive_mutex OUT_STREAM_MUTEX;

	std::string name;
};

/////////////////////////////////////////////////////////////////////////////////////

bool CanLog(SimpleLogger::Level level);

/////////////////////////////////////////////////////////////////////////////////////

class LogChain 
{
	friend class SimpleLogger;
private:
	static const LogChain DISABLED_LOG_CHAIN;
	
	SimpleLogger::Level level;
	bool initial;

	LogChain(SimpleLogger::Level level, std::string loggerName);
public:
	template<typename T>
	LogChain& operator<<(T& token)
	{
		if (initial)
		{
			initial = false;
		}
		else
		{
			std::cout << " ";
		}

		if (CanLog(level))
		{
			std::cout << token;
		}

		return *this;
	}

	template<typename T>
	LogChain& operator<<(T&& token)
	{
		if (initial)
		{
			initial = false;
		}
		else
		{
			std::cout << " ";
		}

		if (CanLog(level))
		{
			std::cout << token;
		}

		return *this;
	}
};

/////////////////////////////////////////////////////////////////////////////////////

SimpleLogger::Level debug();
SimpleLogger::Level info();
SimpleLogger::Level warn();
SimpleLogger::Level error();

/////////////////////////////////////////////////////////////////////////////////////
