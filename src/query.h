#ifndef QUERY_H
#define QUERY_H

#include "util.h"
#include "unordered_set.h"
#include "mutex.h"

struct Query
{
	Query(QueryID id, bud::vector<bud::string>&& str, MatchType match_type, unsigned int tolerance);

	bool operator==(const Query& rhs) const{ return m_id == rhs.m_id; }

	bool operator!=(const Query& rhs) const{ return m_id != rhs.m_id; }

	QueryID m_id;
	bud::vector<bud::string> m_str;

	bud::unordered_set<bud::string> m_str_edit_matched;
	bud::unordered_set<bud::string> m_str_hamming_matched;
	unsigned int exact_matching_matched_words_counter = 0;

	MatchType m_match_type;
	unsigned int m_tolerance;

	bud::mutex m_mtx;
};

#endif // QUERY_H
