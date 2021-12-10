#ifndef IMPL_H
#define IMPL_H

#include "my_string.h"
#include "pair.h"
#include "unique_ptr.h"
#include "unordered_map.h"
#include "unordered_set.h"
#include "util.h"
#include "query.h"

#include "entry.h"
#include "BK_Entry.h"
#include "appMatching/editDistance.h"

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
	ErrorCode addQuery(QueryID id, const char *str, MatchType match_type, unsigned int tolerance);
	ErrorCode removeQuery(QueryID id);
	ErrorCode getNext(DocID *p_doc_id, unsigned int *p_num_res, QueryID **p_query_ids);
	ErrorCode matchDocument(DocID doc_id, const char *doc_str);

private:
	bud::unique_ptr<BK_Entry> m_edit_bk = bud::make_unique<BK_Entry>(BK_Entry(&Edistance));
	bud::unordered_map<bud::string, bud::unordered_set<Query *>> m_words_ht;
	bud::unordered_map<QueryID, Query *> m_queries_ht;
	bud::vector<Result> m_res;
};

#endif // IMPL_H
