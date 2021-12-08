#include "implementation.h"
#include "string_breaker.h"

using bud::unordered_set;

Query::Query(QueryID id, const char* str, MatchType match_type, unsigned int tolerance) :
	m_id(id), m_match_type(match_type), m_tolerance(tolerance)
{
	m_str = string_breaker(str);
}

implementation::~implementation()
{
	for (auto& list : m_queries_ht.data())
	{
		for (auto& query_pair : list)
			delete query_pair.second;
	}
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
	}else if(match_type == MT_EDIT_DIST){
		// m_edit_bk->add(query, tolerance);
		// m_edit_bk->insert(query[0].m_str, tolerance);
		m_edit_bk->insert(&(query->m_str[0]));
	}

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

	if (!m_queries_ht.erase(id))
		return EC_FAIL;

	delete *query;

	return EC_SUCCESS;
}
