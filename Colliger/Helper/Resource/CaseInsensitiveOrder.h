#pragma once

#include <string>

struct CaseInsensitiveOrder
{
	bool operator()(const std::string& s1, const std::string& s2) const;
};
