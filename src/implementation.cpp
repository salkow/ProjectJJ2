#include "implementation.h"
#include "string_breaker.h"
#include <cstring>

Query::Query(QueryID id, const char *str, MatchType match_type, unsigned int tolerance) : m_id(id), m_match_type(match_type), m_tolerance(tolerance)
{
	m_str = string_breaker(str);
}

ErrorCode implementation::addQuery(QueryID id, const char *str, MatchType match_type, unsigned int tolerance)
{
	auto *query = new Query(id, str, match_type, tolerance);

	if (match_type == MT_EXACT_MATCH)
	{
		auto result = m_queries_ht.insert(bud::pair<const QueryID, Query *>(id, query));
		if (!result.second)
			return EC_FAIL;

		for (const auto &query_str : query->m_str)
		{
			bud::vector<Query *> *queries_matching_string = m_words_ht[query_str];

			if (!queries_matching_string)
			{
				bud::vector<Query *> queries;
				queries.emplace_back(query);

				auto other_result =
					m_words_ht.insert(bud::pair<const bud::string, bud::vector<Query *>>(
						query_str, std::move(queries)));

				if (!other_result.second)
					return EC_FAIL;
			}

			else
				queries_matching_string->emplace_back(query);
		}
	}

	return EC_SUCCESS;
}

ErrorCode implementation::removeQuery(QueryID id)
{
	// assuming MT_EXACT_MATCH

	Query **query = m_queries_ht[id];
	if (!query)
		return EC_FAIL;

	for (const auto &query_word : (*query)->m_str)
	{
		bud::vector<Query *> *queries_with_that_word = m_words_ht[query_word];

		if (!queries_with_that_word)
			return EC_FAIL;

		// Erase word if only one query with that word.
		if (queries_with_that_word->size() == 1)
			m_words_ht.erase(query_word);

		else
		{
			auto pos =
				bud::find(queries_with_that_word->begin(), queries_with_that_word->end(), *query);

			queries_with_that_word->erase(pos);
		}
	}

	m_queries_ht.erase(id);

	delete *query;

	return EC_SUCCESS;
}
