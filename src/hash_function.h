#ifndef HASH_FUNCTION_H
#define HASH_FUNCTION_H

#include "my_string.h"

struct Query;

namespace bud
{

template <class T>
struct hash
{
};

template <>
struct hash<int>
{
	std::size_t operator()(int value) const;
};

template <>
struct hash<unsigned int>
{
	std::size_t operator()(unsigned int value) const;
};

template <>
struct hash<string>
{
	std::size_t operator()(const string& value) const;
};

template <>
struct hash<string*>
{
	std::size_t operator()(const string* value) const;
};

template <>
struct hash<Query*>
{
	std::size_t operator()(const Query* value) const;
};

} // namespace bud

#endif // HASH_FUNCTION_H
