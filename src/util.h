#ifndef UTIL_H
#define UTIL_H

#include <cstddef>

#include "my_string.h"

template <typename T>
[[nodiscard]] bool compare_values(const T& first, const T& second)
{
	return first == second;
}

template <typename T>
[[nodiscard]] bool compare_values(T* first, T* second)
{
	return *first == *second;
}

namespace bud
{

template <class InputIt, class UnaryPredicate>
constexpr InputIt find_if(InputIt first, InputIt last, UnaryPredicate p)
{
	for (; first != last; ++first)
	{
		if (p(*first))
			return first;
	}

	return last;
}

} // namespace bud

#endif // UTIL_H
