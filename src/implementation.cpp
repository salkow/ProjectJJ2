#include "implementation.h"

#include <cstdlib>

#include "unordered_set.h"

using bud::string;
using bud::unordered_set;

implementation::implementation()
{
	sem_init(&m_exact_add_sem, 0, 1);
	sem_init(&m_edit_add_sem, 0, 1);
	sem_init(&m_hamming_add_sem, 0, 1);
	sem_init(&m_queries_ht_sem, 0, 1);
}

implementation::~implementation()
{
	m_jm.waitFinishAllJobs();
	sem_destroy(&m_exact_add_sem);
	sem_destroy(&m_edit_add_sem);
	sem_destroy(&m_hamming_add_sem);
	sem_destroy(&m_queries_ht_sem);
	for (auto &query_pair : m_queries_ht)
	{
		delete query_pair.second;
	}
}

ErrorCode implementation::addQuery(QueryID id, bud::vector<string> &&str, MatchType match_type,
								   unsigned int tolerance)
{
	auto *query = new Query(id, std::move(str), match_type, tolerance);

	sem_wait(&m_queries_ht_sem);
	auto result = m_queries_ht.try_emplace(id, query);
	sem_post(&m_queries_ht_sem);
	if (!result.second)
		return EC_FAIL;

	if (match_type == MT_EXACT_MATCH)
	{
		sem_wait(&m_exact_add_sem);
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
		sem_post(&m_exact_add_sem);
	}
	else if (match_type == MT_EDIT_DIST)
	{
		sem_wait(&m_edit_add_sem);
		for (auto &query_str : query->m_str)
		{
			auto returned = m_edit_bk.get(query_str); //check if word already exists
			if (returned != nullptr)				  //if yes
			{
				if (returned->second.size() == 0) // check if its deleted
				{
					m_edit_bk.restore(returned);
				}
				returned->second.insert(query); //append the new array to the set
			}
			else
			{
				auto tpp = new Entry(query_str, bud::unordered_set<Query *>());
				tpp->second.insert(query);
				if (m_edit_bk.insert(tpp) == EC_FAIL)
				{
					return EC_FAIL;
				}
			}
		}
		sem_post(&m_edit_add_sem);
	}
	else if (match_type == MT_HAMMING_DIST)
	{
		sem_wait(&m_hamming_add_sem);
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
		sem_post(&m_hamming_add_sem);
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
			auto returned = m_edit_bk.get(query_str);
			if (returned != nullptr)
			{
				returned->second.erase((*query)); //remove the query from the set
				if (returned->second.size() == 0)
				{
					m_edit_bk.remove(returned); //and "delete" the word if it belongs to no queries.
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

bool implementation::EsearchFilter(const bud::string &word, bud::unordered_set<QueryID> &queries) const
{
	const bud::vector<bud::pair<Entry *, int>> editCurr = m_edit_bk.search(word, 3);
	//	auto tpp = new Entry("query_str", bud::unordered_set<Query*>());
	//	m_edit_bk.insert(tpp);

	bool t = false;
	for (const auto &temp : editCurr)
	{
		for (const auto &tempQuery : temp.first->second)
		{
			if (static_cast<unsigned int>(temp.second) <= tempQuery->m_tolerance)
			{
				tempQuery->m_mtx.lock();
				tempQuery->m_str_edit_matched.insert(temp.first->first);
				if (tempQuery->m_str_edit_matched.size() == tempQuery->m_str.size())
				{
					tempQuery->m_mtx.unlock();
					t = true;
					queries.insert(tempQuery->m_id); //insert it only if every word has been found in the document
				}
				else
				{
					tempQuery->m_mtx.unlock();
				}
			}
		}
	}
	return t;
}

bool implementation::HsearchFilter(const bud::string &word, bud::unordered_set<QueryID> &queries) const
{
	const bud::vector<bud::pair<Entry *, int>> hammingCurr = m_hamming_bk[word.size()].search(word, 3);
	bool t = false;
	for (const auto &temp : hammingCurr)
	{
		for (const auto &tempQuery : temp.first->second)
		{
			if (static_cast<unsigned int>(temp.second) <= tempQuery->m_tolerance)
			{
				tempQuery->m_mtx.lock();
				tempQuery->m_str_hamming_matched.insert(temp.first->first);
				if (tempQuery->m_str_hamming_matched.size() == tempQuery->m_str.size())
				{
					tempQuery->m_mtx.unlock();
					t = true;
					queries.insert(tempQuery->m_id);
				}
				else
				{
					tempQuery->m_mtx.unlock();
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
		query->m_mtx.lock();
		if (++query->exact_matching_matched_words_counter == query->m_str.size())
		{
			query->m_mtx.unlock();
			queries.insert(query->m_id);
		}
		else
		{
			query->m_mtx.unlock();
		}
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

ErrorCode implementation::matchDocument(const bud::vector<string> &words, size_t start, size_t end,
										Result &res) const
{

	if (!(end < words.size()) || !(start <= end))
	{
		auto x = true;
	}
	assert(start <= end);
	assert(end < words.size());
	for (size_t i = start; i <= end; i++)
	{
		searchForExactMatchingWord(words[i], res.m_query_ids);
		EsearchFilter(words[i], res.m_query_ids);
		HsearchFilter(words[i], res.m_query_ids);
	}
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
