#ifndef IMPL_H
#define IMPL_H

#include "vector.h"
#include "../include/core.h"
#include "my_string.h"
#include "unordered_map.h"
#include "unordered_set.h"
#include "util.h"

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
	ErrorCode getNext(DocID* p_doc_id, unsigned int* p_num_res, QueryID** p_query_ids);
	ErrorCode matchDocument(DocID doc_id, const char* doc_str);

private:
	bud::unordered_map<bud::string, bud::unordered_set<Query*>> m_words_ht;
	bud::unordered_map<QueryID, Query*> m_queries_ht;
	bud::vector<Result> m_res;
};

#endif // IMPL_H
