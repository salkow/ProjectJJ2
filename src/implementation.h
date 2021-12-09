#ifndef IMPL_H
#define IMPL_H

#include "my_string.h"
#include "pair.h"
#include "unique_ptr.h"
#include "unordered_map.h"
#include "unordered_set.h"
#include "util.h"

#include "BK_T.h"
#include "appMatching/editDistance.h"

using Entry = bud::pair<bud::string, bud::unordered_set<Query*>>;

struct Query
{
	Query(QueryID id, const char* str, MatchType match_type, unsigned int tolerance);

	bool operator==(const Query& rhs) const { return m_id == rhs.m_id; }
	bool operator!=(const Query& rhs) const { return m_id != rhs.m_id; }

	QueryID m_id;
	bud::unordered_set<bud::string> m_str;
	MatchType m_match_type;
	unsigned int m_tolerance;
};

struct Result
{
	DocID m_doc_id;
	bud::unordered_set<QueryID> m_query_ids;
};

class implementation
{
public:
	implementation() = default;
	~implementation();
	ErrorCode addQuery(QueryID id, const char* str, MatchType match_type, unsigned int tolerance);
	ErrorCode removeQuery(QueryID id);
	ErrorCode getNext(DocID* p_doc_id, unsigned int* p_num_res, QueryID** p_query_ids);
	ErrorCode matchDocument(DocID doc_id, const char* doc_str);

	bud::unique_ptr<BK<bud::string>> m_edit_bk =
		bud::make_unique<BK<bud::string>>(BK(&Edistance)); // TODO: make private

private:
	bud::unordered_map<bud::string, bud::unordered_set<Query*>> m_words_ht;
	bud::unordered_map<QueryID, Query*> m_queries_ht;
	bud::vector<Result> m_res;
};

#endif // IMPL_H
