#include "../include/core.h"
#include "implementation.h"
#include "string_breaker.h"

using bud::string;

namespace
{
	implementation impl;
}

ErrorCode InitializeIndex() { return EC_SUCCESS; }

ErrorCode DestroyIndex() { return EC_SUCCESS; }

ErrorCode StartQuery(QueryID query_id, const char *query_str, MatchType match_type,
					 unsigned int match_dist)
{
	ErrorCode err = EC_SUCCESS;
	bud::vector<string> words = string_breaker(query_str);

	impl.m_jm.addJob(Job([&, query_id, match_type, match_dist, words = std::move(words)]() mutable
						 { impl.addQuery(query_id, std::move(words), match_type, match_dist); }));

	return err;
}

ErrorCode EndQuery(QueryID query_id)
{
	impl.m_jm.waitFinishAllJobs();
	return impl.removeQuery(query_id);
}

ErrorCode MatchDocument(DocID doc_id, const char *doc_str)
{
	const bud::vector<string> words = string_breaker(doc_str);

	const size_t real_num_of_threads = bud::min(static_cast<size_t>(NUM_OF_THREADS), words.size() - 1);
	size_t split = words.size() / real_num_of_threads;
	size_t start = 0;
	size_t end = split;

	bud::array<Result, NUM_OF_THREADS> res;
	bud::array<ErrorCode, NUM_OF_THREADS> err;

	impl.m_jm.waitFinishAllJobs();

	size_t i;
	for (i = 0; i < real_num_of_threads - 1; i++)
	{
		impl.m_jm.addJob(Job([&, start, end, i]() { //TODO words
			err[i] = impl.matchDocument(words, start, end, res[i]);
		}));

		start = end + 1;
		end += split;
	}

	impl.m_jm.addJob(Job([&, start, real_num_of_threads]()
						 {
							 err[real_num_of_threads - 1] = impl.matchDocument(words, start, words.size() - 1, res[real_num_of_threads - 1]);
						 }));

	impl.m_jm.waitFinishAllJobs();
	Result fin_res;
	fin_res.m_doc_id = doc_id;
	for (size_t i2 = 0; i2 < real_num_of_threads; i2++)
	{
		const auto &item = res[i2];
		for (const auto &x : item.m_query_ids)
		{
			fin_res.m_query_ids.insert(x);
		}
	}

	impl.m_res.emplace_back(std::move(fin_res));

	impl.queries_matched_words_reset();
	return EC_SUCCESS;
}

ErrorCode GetNextAvailRes(DocID *p_doc_id, unsigned int *p_num_res, QueryID **p_query_ids)
{
	impl.m_jm.waitFinishAllJobs();

	return impl.getNext(p_doc_id, p_num_res, p_query_ids);
}
