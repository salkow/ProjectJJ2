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
	ErrorCode err = EC_SUCCESS;
	bud::vector<string> words = string_breaker(query_str);

	impl.m_jm.addJob(Job([&, query_id, match_type, match_dist, words = std::move(words)]()mutable{
		impl.addQuery(query_id, std::move(words), match_type, match_dist);
	}));
	return err;
}

ErrorCode EndQuery(QueryID query_id){
	impl.m_jm.waitFinishAllJobs();
	assert(impl.m_jm.num_of_jobs() == 0);
	return impl.removeQuery(query_id);
}

ErrorCode MatchDocument(DocID doc_id, const char*doc_str){
	const bud::vector<string> words = string_breaker(doc_str);
//	assert(words.size() == 1066);

	size_t split = words.size()/NUM_OF_THREADS;
	size_t start = 0;
	size_t end = split;

	bud::array<Result, NUM_OF_THREADS> res;

	impl.m_jm.waitFinishAllJobs();
	assert(impl.m_jm.num_of_jobs() == 0);

	bud::array<ErrorCode, NUM_OF_THREADS> err;

	size_t i;
	for(i = 0;i < NUM_OF_THREADS-1;i++){
		impl.m_jm.addJob(Job([&, start, end](){//words
			err[i] = impl.matchDocument(words, start, end, res[i]);
		}));
		start += split+1;
		end += split;
	}
	impl.m_jm.addJob(Job([&, start](){
//		assert(words.size() == 1066);
//		assert(words.size()-1 == end);
		err[i] = impl.matchDocument(words, start, words.size()-1, res[i]);
	}));

//	assert(impl.m_jm.num_of_jobs() != 0);
	impl.m_jm.waitFinishAllJobs();
	assert(impl.m_jm.num_of_jobs() == 0);
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

ErrorCode GetNextAvailRes(DocID* p_doc_id, unsigned int* p_num_res, QueryID** p_query_ids){
	impl.m_jm.waitFinishAllJobs();

	return impl.getNext(p_doc_id, p_num_res, p_query_ids);
}
