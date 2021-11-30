#ifndef UTIL_H
#define UTIL_H

#include <cstddef>

#include "my_string.h"
#include "vector.h"
#include "../include/core.h"

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

struct HashFunction
{
	std::size_t operator()(const bud::string& value) const
	{ 
		std::size_t sum = 0;

		for (const auto I : value)
			sum += static_cast<std::size_t>(I);

		return sum;
	}

	std::size_t operator()(const bud::string* value) const
	{
		std::size_t sum = 0;

		for (const auto I : *value)
			sum += static_cast<std::size_t>(I);

		return sum;
	}

	std::size_t operator()(int value) const { return static_cast<size_t>(value); }
};

struct Query {
    Query(QueryID id, const char* str, MatchType match_type, unsigned int tolerance);
    QueryID                     m_id;
    bud::vector<bud::string>    m_str;
    MatchType                   m_match_type;
    unsigned int                m_tolerance;
};

struct Result{
    DocID         m_doc_id;
    unsigned int  m_num_res;
    QueryID*      m_query_ids;
};

#endif // UTIL_H
