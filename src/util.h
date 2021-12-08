#ifndef UTIL_H
#define UTIL_H

#include <cstddef>

#include "my_string.h"
#include "vector.h"

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

struct HashFunction
{
	std::size_t operator()(const bud::string &value) const
	{
		std::size_t sum = 0;

		for (const auto I : value)
			sum += static_cast<std::size_t>(I);

		return sum;
	}

	std::size_t operator()(const bud::string *value) const
	{
		std::size_t sum = 0;

		for (const auto I : *value)
			sum += static_cast<std::size_t>(I);

		return sum;
	}

	std::size_t operator()(int value) const { return static_cast<size_t>(value); }

	std::size_t operator()(unsigned int value) const { return static_cast<size_t>(value); }
};

namespace bud
{

	template <class InputIt, class T>
	InputIt find(InputIt first, InputIt last, const T &value)
	{
		for (; first != last; ++first)
		{
			if (*first == value)
				return first;
		}

		return last;
	}

} // namespace bud

#endif // UTIL_H
