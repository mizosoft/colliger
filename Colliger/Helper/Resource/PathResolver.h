#pragma once

#include <string>

class PathResolver
{
public:
	PathResolver() = default;
	virtual ~PathResolver() = default;

	virtual std::string Resolve(const std::string& unresolved) const;
};
