#include "../include/core.h"
#include "implementation.h"
#include "string_breaker.h"

using bud::string;

namespace{
	implementation impl;
}


ErrorCode InitializeIndex(){ return EC_SUCCESS; }

ErrorCode DestroyIndex(){ return EC_SUCCESS; }

ErrorCode StartQuery(QueryID query_id, const char*query_str, MatchType match_type,
					 unsigned int match_dist){
	ErrorCode err;
	Job jb([&](){
		err = impl.addQuery(query_id, query_str, match_type, match_dist);
	});
	impl.m_jm.addJob(std::move(jb));
	return err;
}

ErrorCode EndQuery(QueryID query_id) { return impl.removeQuery(query_id); }

ErrorCode MatchDocument(DocID doc_id, const char*doc_str){
	bud::vector<string> words = string_breaker(doc_str);

	size_t split = words.size()/NUM_OF_THREADS;
	size_t start = 0;
	size_t end = split;

	bud::array<Result, NUM_OF_THREADS> res;

	impl.m_jm.waitFinishAllJobs();

	bud::array<ErrorCode, NUM_OF_THREADS> err;

	size_t i;
	for(i = 0;i < NUM_OF_THREADS-1;i++){
		impl.m_jm.addJob(Job([&](){
			err[i] = impl.matchDocument(words, start, end, res[i]);
		}));
		start += split+1;
		end += split;
	}
	impl.m_jm.addJob(Job([&](){
		err[i] = impl.matchDocument(words, start, words.size(), res[i]);
	}));

	impl.m_jm.waitFinishAllJobs();

	Result fin_res;
	fin_res.m_doc_id = doc_id;
	for(const auto& item : res){
		for(const auto& x : item.m_query_ids){
			fin_res.m_query_ids.insert(x);
		}
	}


	impl.queries_matched_words_reset();

	for(auto item : err){
		if(item == EC_FAIL){
			return EC_FAIL;
		}
	}
	return EC_SUCCESS;
}

ErrorCode GetNextAvailRes(DocID* p_doc_id, unsigned int* p_num_res, QueryID** p_query_ids)
{
	return impl.getNext(p_doc_id, p_num_res, p_query_ids);
}
