#ifndef UTIL_H
#define UTIL_H

#include <cstddef>

#include "my_string.h"
#include "vector.h"
#include "../include/core.h"

template <typename T>
[[nodiscard]] bool compare_values(const T &first, const T &second)
{
	return first == second;
}

template <typename T>
[[nodiscard]] bool compare_values(T *first, T *second)
{
	return *first == *second;
}

namespace bud
{

	template <class InputIt, class T>
	constexpr InputIt find(InputIt first, InputIt last, const T &value)
	{
		for (; first != last; ++first)
		{
			if (*first == value)
				return first;
		}
		return last;
	}

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

	template <typename First, typename Second, typename... Other>
	constexpr auto min(First &&first, Second &&second, Other &&...other)
	{
		if constexpr (sizeof...(other) == 0)
			return first < second ? first : second;
		else
			return min(min(first, second), other...);
	}

	template <typename First, typename Second, typename... Other>
	constexpr auto max(First &&first, Second &&second, Other &&...other)
	{
		if constexpr (sizeof...(other) == 0)
			return first > second ? first : second;
		else
			return max(max(first, second), other...);
	}
} // namespace bud

#endif // UTIL_H
