#include "implementation.h"
#include "entry.h"
#include "my_string.h"
#include "pair.h"
#include "string_breaker.h"
#include "unordered_set.h"

using bud::string;
using bud::unordered_set;

implementation::~implementation()
{
	for (auto &list : m_queries_ht.data())
	{
		for (auto &query_pair : list)
			delete query_pair.second;
	}
}

ErrorCode implementation::addQuery(QueryID id, const char *str, MatchType match_type,
								   unsigned int tolerance)
{
	auto *query = new Query(id, str, match_type, tolerance);

	auto result = m_queries_ht.try_emplace(id, query);
	if (!result.second)
		return EC_FAIL;

	if (match_type == MT_EXACT_MATCH)
	{
		for (auto &bucket : (query)->m_str.data())
		{
			for (auto &query_str : bucket)
			{
				unordered_set<Query *> *matching_queries = m_words_ht[query_str];

				if (!matching_queries)
				{
					unordered_set<Query *> new_queries;
					new_queries.insert(query);

					auto other_result = m_words_ht.try_emplace(query_str, std::move(new_queries));

					if (!other_result.second)
						return EC_FAIL;
				}

				else
					matching_queries->insert(query);
			}
		}
	}
	else if (match_type == MT_EDIT_DIST)
	{
		for (auto& bucket : (query)->m_str.data())
		{
			for (auto &query_str : bucket)
			{
				auto returned = m_edit_bk->get(query_str);
				if(returned != nullptr){
					returned->second.insert(query);
				}else{
					auto tpp = new Entry(query_str, bud::unordered_set<Query *>());
					tpp->second.insert(query);
					if(m_edit_bk->insert(tpp) == EC_FAIL){
						return EC_FAIL;
					}
				}
			}
		}
	}
	return EC_SUCCESS;
}

ErrorCode implementation::removeQuery(QueryID id)
{
	Query **query = m_queries_ht[id];
	if (!query)
		return EC_FAIL;

	if ((*query)->m_match_type == MT_EXACT_MATCH) {
		for (auto &bucket: (*query)->m_str.data()) {
			for (auto &query_word: bucket) {
				unordered_set<Query *> *queries_with_that_word = m_words_ht[query_word];
				if (!queries_with_that_word)
					return EC_FAIL;

				if (queries_with_that_word->size() == 1)
					m_words_ht.erase(query_word);

				else
					queries_with_that_word->erase(*query);
			}
		}
	}else if((*query)->m_match_type == MT_EDIT_DIST){
		for (auto &bucket: (*query)->m_str.data()) {
			for (auto &query_str: bucket) {
				auto returned = m_edit_bk->get(query_str);
				if(returned != nullptr) {
					returned->second.erase((*query));
					if (returned->second.size() == 0) {
						m_edit_bk->remove(returned);
					}
				}
			}
		}
	}

	if (!m_queries_ht.erase(id))
		return EC_FAIL;

	delete *query;

	return EC_SUCCESS;
}

ErrorCode implementation::matchDocument(DocID doc_id, const char *doc_str)
{
	unordered_set<string> words = string_breaker(doc_str);

	Result result;
	result.m_doc_id = doc_id;

	for (auto &bucket : words.data())
	{
		for (auto &word : bucket)
		{
			// Search with EXACT MATCHING.

			// Search with HAMMING DISTANCE.

			// Search with EDIT DISTANCE.

			result.m_query_ids.insert(1);
		}
	}

	return EC_SUCCESS;
}

ErrorCode implementation::getNext(DocID *p_doc_id, unsigned int *p_num_res, QueryID **p_query_ids)
{
	if (m_res.size() == 0)
		return EC_NO_AVAIL_RES;

	// TODO: Get the first/last element from the unordered_set.
	*p_doc_id = m_res.back().m_doc_id;
	*p_num_res = static_cast<unsigned int>(m_res.back().m_query_ids.size());

	//	p_query_ids = new QueryID(*p_num_res);

	//		*p_query_ids = m_res.back().m_query_ids;

	m_res.pop_back();
	return EC_SUCCESS;
}
