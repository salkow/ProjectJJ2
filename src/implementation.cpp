#include "implementation.h"
#include "string_breaker.h"
#include "unordered_set.h"

using bud::string;
using bud::unordered_set;

implementation::~implementation()
{
	for (auto& query_pair : m_queries_ht)
		delete query_pair.second;
}

ErrorCode implementation::addQuery(QueryID id, const char* str, MatchType match_type,
								   unsigned int tolerance)
{
	auto* query = new Query(id, str, match_type, tolerance);

	auto result = m_queries_ht.try_emplace(id, query);
	if (!result.second)
		return EC_FAIL;

	if (match_type == MT_EXACT_MATCH)
	{
		for (const auto& query_str : query->m_str)
		{
			unordered_set<Query*>* matching_queries = m_words_ht[query_str];

			if (!matching_queries)
			{
				unordered_set<Query*> new_queries;
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
		for (auto& bucket : (query)->m_str.data())
		{
			for (auto& query_str : bucket)
			{
				auto returned = m_edit_bk->get(query_str);
				if (returned != nullptr)
				{
					returned->second.insert(query);
				}
				else
				{
					auto tpp = new Entry(query_str, bud::unordered_set<Query*>());
					tpp->second.insert(query);
					if (m_edit_bk->insert(tpp) == EC_FAIL)
					{
						return EC_FAIL;
					}
				}
			}
		}
	}

	//}

	return EC_SUCCESS;
}

ErrorCode implementation::removeQuery(QueryID id)
{
	Query** query = m_queries_ht[id];
	if (!query)
		return EC_FAIL;

	if ((*query)->m_match_type == MT_EXACT_MATCH)
	{
		for (const auto& query_word : (*query)->m_str)
		{
			unordered_set<Query*>* queries_with_that_word = m_words_ht[query_word];
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
		for (auto& bucket : (*query)->m_str.data())
		{
			for (auto& query_str : bucket)
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
	}

	if (!m_queries_ht.erase(id))
		return EC_FAIL;

	delete *query;

	return EC_SUCCESS;
}

bool implementation::searchFilter(const bud::string& word, bud::unordered_set<QueryID>& queries)
{
	bud::vector<bud::pair<Entry*, int>> editCurr = m_edit_bk->search(word, 3);
	bool t = false;
	for (auto& temp : editCurr)
	{
		for (auto& tempQuery : temp.first->second)
		{
			if (tempQuery->m_tolerance <= unsigned(temp.second))
			{
				t = true;
				queries.insert(tempQuery->m_id);
			}
		}
	}
	return t;
}

bool implementation::searchForExactMatchingWord(const string& word,
												unordered_set<QueryID>& queries) const
{
	const unordered_set<Query*>* matching_queries = m_words_ht[word];
	if (!matching_queries)
		return false;

	for (auto& query : *matching_queries)
	{
		if (++query->exact_matching_matched_words_counter == query->m_str.size())
			queries.insert(query->m_id);
	}

	return true;
}

void implementation::exact_matching_reset_matched_counter()
{
	for (auto& query : m_queries_ht)
		query.second->exact_matching_matched_words_counter = 0;
}

ErrorCode implementation::matchDocument(DocID doc_id, const char* doc_str)
{
	if (!doc_str)
		return EC_FAIL;

	unordered_set<string> words = string_breaker(doc_str);

	Result res;
	res.m_doc_id = doc_id;

	for (const auto& word : words)
	{
		bool found = false;

		found |= searchForExactMatchingWord(word, res.m_query_ids);

		// Search with EDIT DISTANCE.
		found |= searchFilter(word, res.m_query_ids);

		if (!found)
			break;
	}

	m_res.emplace_back(std::move(res));

	exact_matching_reset_matched_counter();

	return EC_SUCCESS;
}
ErrorCode implementation::getNext(DocID* p_doc_id, unsigned int* p_num_res, QueryID** p_query_ids)
{
	if (m_res.size() == 0)
		return EC_NO_AVAIL_RES;

	const auto& last_result = m_res.back();

	*p_doc_id = last_result.m_doc_id;
	*p_num_res = static_cast<unsigned int>(last_result.m_query_ids.size());

	*p_query_ids = static_cast<QueryID*>(malloc(sizeof(QueryID) * *p_num_res));

	// TODO: We need to sort them.

	int i = 0;
	for (const auto& query_id : last_result.m_query_ids)
	{
		(*p_query_ids)[i] = query_id;
		++i;
	}

	m_res.pop_back();
	return EC_SUCCESS;
}
