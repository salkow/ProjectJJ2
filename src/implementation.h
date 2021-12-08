#ifndef IMPL_H
#define IMPL_H

#include "unique_ptr.h"
#include "unordered_map.h"
#include "unordered_set.h"
#include "util.h"

#include "BK_T.h"
#include "appMatching/editDistance.h"

struct Query
{
	Query(QueryID id, const char* str, MatchType match_type, unsigned int tolerance);

	bool operator==(const Query& rhs) const { return m_id == rhs.m_id; }
	bool operator!=(const Query& rhs) const { return m_id != rhs.m_id; }

	QueryID m_id;
	bud::vector<bud::string> m_str;
	MatchType m_match_type;
	unsigned int m_tolerance;
};

struct Result
{
	DocID m_doc_id;
	unsigned int m_num_res;
	QueryID* m_query_ids;
};

struct Entry
{
	bud::string m_word;
	bud::vector<Query*> m_payload;
};

class implementation
{
public:
	implementation() = default;
	~implementation();
	ErrorCode addQuery(QueryID id, const char* str, MatchType match_type, unsigned int tolerance);
	ErrorCode removeQuery(QueryID id);
	ErrorCode getNext(DocID* p_doc_id, unsigned int* p_num_res, QueryID** p_query_ids)
	{
		if (m_res.size() == 0)
		{
			return EC_NO_AVAIL_RES;
		}
		*p_doc_id = m_res.back().m_doc_id;
		*p_num_res = m_res.back().m_num_res;
		*p_query_ids = m_res.back().m_query_ids;

		m_res.pop_back();
		return EC_SUCCESS;
	}

private:
	bud::unordered_map<bud::string, bud::unordered_set<Query*>> m_words_ht;
	bud::unordered_map<QueryID, Query*> m_queries_ht;
	bud::vector<Result> m_res;

	bud::unique_ptr<BK<bud::string>> m_edit_bk = bud::make_unique<BK<bud::string>>(BK(&Edistance));
};

#endif // IMPL_H
