#include "SimpleLogger.h"

std::string GetLevelString(SimpleLogger::Level level)
{
	switch (level)
	{
	case SimpleLogger::DEBUG:
		return "[DEBUG]";
	case SimpleLogger::INFO:
		return "[INFO]";
	case SimpleLogger::WARN:
		return "[WARN]";
	case SimpleLogger::ERROR:
		return "[ERROR]";
	default:
		return ""; // ??
	}
}

bool CanLog(SimpleLogger::Level level)
{
	return level > SimpleLogger::OFF && level >= SimpleLogger::GLOBAL_LEVEL;
}

// ----------SimpleLogger----------

bool SimpleLogger::initial = true;
std::recursive_mutex SimpleLogger::OUT_STREAM_MUTEX;
const SimpleLogger::Level SimpleLogger::GLOBAL_LEVEL = SimpleLogger::Level::DEBUG;

SimpleLogger::SimpleLogger(std::string name) : 
	name(name)
{ 
}

LogChain SimpleLogger::operator<<(SimpleLogger::Level level) const
{
	if (CanLog(level))
	{
		if (initial)
		{
			initial = false;
		}
		else
		{
			std::cout << std::endl;
		}
	}

	return level > SimpleLogger::OFF ? LogChain(level, name) : LogChain::DISABLED_LOG_CHAIN;
}

// ------------LogChain------------

const LogChain LogChain::DISABLED_LOG_CHAIN = LogChain(SimpleLogger::OFF, "");

LogChain::LogChain(SimpleLogger::Level level, std::string loggerName) : level(level), initial(true)
{
	if (CanLog(level))
	{
		std::cout << GetLevelString(level) << "[" << loggerName << "]: ";
	}
}

SimpleLogger::Level debug()
{
	return SimpleLogger::DEBUG;
}

SimpleLogger::Level info()
{
	return SimpleLogger::INFO;
}

SimpleLogger::Level warn()
{
	return SimpleLogger::WARN;
}

SimpleLogger::Level error()
{
	return SimpleLogger::ERROR;
}
