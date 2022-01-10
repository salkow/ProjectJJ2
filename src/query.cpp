#include "query.h"
#include "string_breaker.h"


Query::Query(QueryID id, bud::vector<bud::string>&& str, MatchType match_type, unsigned int tolerance):
		m_id(id), m_match_type(match_type), m_tolerance(tolerance){
	m_str = std::move(str);
}
