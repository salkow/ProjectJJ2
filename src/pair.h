#ifndef PAIR_H
#define PAIR_H

#include <utility>

namespace bud
{

template <class T1, class T2>
struct pair
{
	pair() = default;
	pair(const pair& p) = default;
	pair(pair&& p) noexcept = default;

	pair(const T1& x, const T2& y) : first(x), second(y) {}

	template <class U1, class U2>
	pair(U1&& x, U2&& y) : first(std::forward<U1>(x)), second(std::forward<U2>(y))
	{
	}

	template <class U1, class U2>
	pair(const pair<U1, U2>& p) : first(p.first), second(p.second)
	{
	}

	template <class U1, class U2>
	pair(pair<U1, U2>&& p) noexcept :
		first(std::forward<U1>(p.first)), second(std::forward<U2>(p.second))
	{
	}

	pair& operator=(const pair& other)
	{
		first = other.first;
		second = other.second;

		return *this;
	}

	pair& operator=(pair&& other) noexcept
	{
		first = std::move(other.first);
		second = std::move(other.second);

		return *this;
	}

	T1 first;
	T2 second;
};

} // namespace bud

#endif // PAIR_H
