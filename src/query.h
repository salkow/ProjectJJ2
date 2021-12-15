#ifndef QUERY_H
#define QUERY_H

#include "util.h"
#include "unordered_set.h"

struct Query
{
	Query(QueryID id, const char* str, MatchType match_type, unsigned int tolerance);

	bool operator==(const Query& rhs) const { return m_id == rhs.m_id; }
	bool operator!=(const Query& rhs) const { return m_id != rhs.m_id; }

	QueryID m_id;
	bud::unordered_set<bud::string> m_str;
	bud::unordered_set<bud::string> m_str_edit_matched;
	bud::unordered_set<bud::string> m_str_hamming_matched;
	MatchType m_match_type;
	unsigned int m_tolerance;

	unsigned int exact_matching_matched_words_counter = 0;
};

#endif // QUERY_H
