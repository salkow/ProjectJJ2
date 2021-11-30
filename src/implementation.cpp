#include "implementation.h"

Query::Query(QueryID id, const char* str, MatchType match_type, unsigned int tolerance) :
	m_id(id), m_match_type(match_type), m_tolerance(tolerance)
{
	// TODO: Split str into words and add them to m_str.
	// HINT: Replace ' ' with \0 and use the string constructor to get the word.
}