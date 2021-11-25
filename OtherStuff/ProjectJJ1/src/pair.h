#ifndef PAIR_H
#define PAIR_H

#include <utility>

namespace bud
{

template <class T1, class T2>
struct pair
{
	pair() = default;
	pair(T1 x, T2 y) : first(std::move(x)), second(std::move(y)) {}

	T1 first;
	T2 second;
};

} // namespace bud

#endif // PAIR_H
