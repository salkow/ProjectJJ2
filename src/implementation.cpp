#include "implementation.h"

#include <cstdlib>

#include "string_breaker.h"
#include "unordered_set.h"

using bud::string;
using bud::unordered_set;

implementation::~implementation()
{
	for (auto &query_pair : m_queries_ht)
	{
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
		for (const auto &query_str : query->m_str)
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
	else if (match_type == MT_EDIT_DIST)
	{
		for (auto &query_str : query->m_str)
		{
			auto returned = m_edit_bk->get(query_str);
			if (returned != nullptr)
			{
				if (returned->second.size() == 0)
				{
					m_edit_bk->restore(returned);
				}
				returned->second.insert(query);
			}
			else
			{
				auto tpp = new Entry(query_str, bud::unordered_set<Query *>());
				tpp->second.insert(query);
				if (m_edit_bk->insert(tpp) == EC_FAIL)
				{
					return EC_FAIL;
				}
			}
		}
	}
	else if (match_type == MT_HAMMING_DIST)
	{
		for (auto &query_str : query->m_str)
		{
			auto returned = m_hamming_bk[query_str.size()].get(query_str);
			if (returned != nullptr)
			{
				if (returned->second.size() == 0)
				{
					m_hamming_bk[query_str.size()].restore(returned);
				}
				returned->second.insert(query);
			}
			else
			{
				auto tpp = new Entry(query_str, bud::unordered_set<Query *>());
				tpp->second.insert(query);
				if (m_hamming_bk[query_str.size()].insert(tpp) == EC_FAIL)
				{
					return EC_FAIL;
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

	if ((*query)->m_match_type == MT_EXACT_MATCH)
	{
		for (const auto &query_word : (*query)->m_str)
		{
			unordered_set<Query *> *queries_with_that_word = m_words_ht[query_word];
			if (!queries_with_that_word)
				return EC_FAIL;

			if (queries_with_that_word->size() == 1)
				m_words_ht.erase(query_word);

			else
				queries_with_that_word->erase(*query);
		}
	}
	else if ((*query)->m_match_type == MT_EDIT_DIST)
	{
		for (auto &query_str : (*query)->m_str)
		{
			auto returned = m_edit_bk->get(query_str);
			if (returned != nullptr)
			{
				returned->second.erase((*query));
				if (returned->second.size() == 0)
				{
					m_edit_bk->remove(returned);
				}
			}
		}
	}
	else if ((*query)->m_match_type == MT_HAMMING_DIST)
	{
		for (auto &query_str : (*query)->m_str)
		{
			auto returned = m_hamming_bk[query_str.size()].get(query_str);
			if (returned != nullptr)
			{
				returned->second.erase((*query));
				if (returned->second.size() == 0)
				{
					m_hamming_bk[query_str.size()].remove(returned);
				}
			}
		}
	}

	Query *q = *query;

	if (!m_queries_ht.erase(id))
		return EC_FAIL;

	delete q;

	return EC_SUCCESS;
}

bool implementation::EsearchFilter(bud::string &word, bud::unordered_set<QueryID> &queries)
{
	bud::vector<bud::pair<Entry *, int>> editCurr = m_edit_bk->search(word, 3);
	bool t = false;
	for (auto &temp : editCurr)
	{
		for (auto &tempQuery : temp.first->second)
		{
			if (static_cast<unsigned int>(temp.second) <= tempQuery->m_tolerance)
			{
				tempQuery->m_str_edit_matched.insert(temp.first->first);
				if (tempQuery->m_str_edit_matched.size() == tempQuery->m_str.size())
				{
					t = true;
					queries.insert(tempQuery->m_id);
				}
			}
		}
	}
	return t;
}

bool implementation::HsearchFilter(bud::string &word, bud::unordered_set<QueryID> &queries)
{
	bud::vector<bud::pair<Entry *, int>> hammingCurr = m_hamming_bk[word.size()].search(word, 3);
	bool t = false;
	for (auto &temp : hammingCurr)
	{
		for (auto &tempQuery : temp.first->second)
		{
			if (static_cast<unsigned int>(temp.second) <= tempQuery->m_tolerance)
			{
				tempQuery->m_str_hamming_matched.insert(temp.first->first);
				if (tempQuery->m_str_hamming_matched.size() == tempQuery->m_str.size())
				{
					t = true;
					queries.insert(tempQuery->m_id);
				}
			}
		}
	}
	return t;
}

bool implementation::searchForExactMatchingWord(const string &word,
												unordered_set<QueryID> &queries) const
{
	const unordered_set<Query *> *matching_queries = m_words_ht[word];
	if (!matching_queries)
		return false;

	for (auto &query : *matching_queries)
	{
		if (++query->exact_matching_matched_words_counter == query->m_str.size())
			queries.insert(query->m_id);
	}

	return true;
}

void implementation::queries_matched_words_reset()
{
	for (auto &query : m_queries_ht)
	{
		query.second->exact_matching_matched_words_counter = 0;
		query.second->m_str_edit_matched.clear();
		query.second->m_str_hamming_matched.clear();
	}
}

ErrorCode implementation::matchDocument(DocID doc_id, const char *doc_str)
{
	if (!doc_str)
		return EC_FAIL;

	Result res;
	res.m_doc_id = doc_id;

	unordered_set<string> words = string_breaker(doc_str);
	for (auto &word : words)
	{
		bool found = false;

		found |= searchForExactMatchingWord(word, res.m_query_ids);
		found |= EsearchFilter(word, res.m_query_ids);
		found |= HsearchFilter(word, res.m_query_ids);
	}

	m_res.emplace_back(std::move(res));
	queries_matched_words_reset();

	return EC_SUCCESS;
}
ErrorCode implementation::getNext(DocID *p_doc_id, unsigned int *p_num_res, QueryID **p_query_ids)
{

	if (m_res.size() == 0)
		return EC_NO_AVAIL_RES;

	const auto &last_result = m_res.back();

	*p_doc_id = last_result.m_doc_id;
	*p_num_res = static_cast<unsigned int>(last_result.m_query_ids.size());

	*p_query_ids = static_cast<QueryID *>(malloc(sizeof(QueryID) * *p_num_res));

	int i = 0;
	for (const auto &query_id : last_result.m_query_ids)
	{
		(*p_query_ids)[i] = query_id;
		++i;
	}

	std::qsort(*p_query_ids, *p_num_res, sizeof(QueryID),
			   [](const void *first_id, const void *second_id)
			   {
				   const QueryID id_1 = *static_cast<const QueryID *>(first_id);
				   const QueryID id_2 = *static_cast<const QueryID *>(second_id);

				   return static_cast<int>(id_1 - id_2);
			   });

	m_res.pop_back();
	return EC_SUCCESS;
}
