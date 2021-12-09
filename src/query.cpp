#include "query.h"
#include "string_breaker.h"

Query::Query(QueryID id, const char *str, MatchType match_type, unsigned int tolerance) : m_id(id), m_match_type(match_type), m_tolerance(tolerance)
{
	m_str = string_breaker(str);
}
