#ifndef _QUERY_
#define _QUERY_

#include "util.h"
#include "unordered_set.h"

struct Query
{
	Query(QueryID id, const char *str, MatchType match_type, unsigned int tolerance);

	bool operator==(const Query &rhs) const { return m_id == rhs.m_id; }
	bool operator!=(const Query &rhs) const { return m_id != rhs.m_id; }

	QueryID m_id;
	bud::unordered_set<bud::string> m_str;
	MatchType m_match_type;
	unsigned int m_tolerance;
};

#endif
