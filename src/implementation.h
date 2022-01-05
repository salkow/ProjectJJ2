#ifndef IMPL_H
#define IMPL_H

#include "my_string.h"
#include "pair.h"
#include "unique_ptr.h"
#include "unordered_map.h"
#include "unordered_set.h"
#include "util.h"
#include "query.h"
#include "job_manager.h"

#include "entry.h"
#include "BK_Entry.h"
#include "appMatching/editDistance.h"
#include "../src/array.h"
#include "vector.h"
#include "appMatching/hammingDistance.h"
#include "unistd.h"
#include "semaphore.h"

struct Result
{
	DocID m_doc_id;
	bud::unordered_set<QueryID> m_query_ids;
};

class implementation
{
public:
	implementation();

	~implementation();

	ErrorCode addQuery(QueryID id, const char *str, MatchType match_type, unsigned int tolerance);

	ErrorCode removeQuery(QueryID id);

	ErrorCode getNext(DocID *p_doc_id, unsigned int *p_num_res, QueryID **p_query_ids);

	ErrorCode matchDocument(const bud::vector<bud::string> &words, size_t start, size_t end, Result &res);

	void queries_matched_words_reset();

	JobManager m_jm;

private:
	bool searchForExactMatchingWord(const bud::string &word,
									bud::unordered_set<QueryID> &queries) const;

	bool EsearchFilter(const bud::string &word, bud::unordered_set<QueryID> &queries);

	bool HsearchFilter(const bud::string &word, bud::unordered_set<QueryID> &queries);

	bud::unique_ptr<BK_Entry> m_edit_bk = bud::make_unique<BK_Entry>(BK_Entry(&Edistance));

	bud::vector<BK_Entry> m_hamming_bk =
		bud::vector<BK_Entry>(MAX_WORD_LENGTH, BK_Entry(&Hdistance)); // TODO: MIN_WORD_LENGTH

	bud::vector<Result> m_res;

	bud::unordered_map<bud::string, bud::unordered_set<Query *>> m_words_ht;
	bud::unordered_map<QueryID, Query *> m_queries_ht;

	sem_t m_exact_add_sem;
	sem_t m_edit_add_sem;
	sem_t m_hamming_add_sem;
	sem_t m_queries_ht_sem;
};
#endif // IMPL_H
