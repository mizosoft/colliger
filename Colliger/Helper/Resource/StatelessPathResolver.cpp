#include "StatelessPathResolver.h"

#include <string>

StatelessPathResolver::StatelessPathResolver(const ResolverFunc& func) :
	func(func)
{
}

std::string StatelessPathResolver::Resolve(const std::string& unresolved) const
{
	return func(unresolved);
}
