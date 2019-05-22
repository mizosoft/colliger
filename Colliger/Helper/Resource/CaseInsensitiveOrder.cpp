#include "CaseInsensitiveOrder.h"

#include "Helper/Utils/Math.h"

bool CaseInsensitiveOrder::operator()(const std::string& s1, const std::string& s2) const
{
	size_t sz1 = s1.length(), sz2 = s2.length();
	size_t sz = Min(sz1, sz2);
	for (size_t i = 0; i < sz; i++)
	{
		int c = std::tolower(s1[i]) - std::tolower(s2[i]);
		if (c != 0)
		{
			return c < 0 ? true : false;
		}
	}

	return sz1 < sz2 ? true : false;
}
