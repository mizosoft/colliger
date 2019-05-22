#pragma once

#include <functional>

#include "PathResolver.h"

class StatelessPathResolver :
	public PathResolver
{
public:
	using ResolverFunc = std::function<std::string(const std::string&)>;

	StatelessPathResolver(const ResolverFunc& func);
	~StatelessPathResolver() = default;

	virtual std::string Resolve(const std::string& unresolved) const override;
private:
	ResolverFunc func;
};
