#include "../include/core.h"
#include "implementation.h"

namespace
{
implementation impl;
}

ErrorCode InitializeIndex() { return EC_SUCCESS; }

ErrorCode DestroyIndex() { return EC_SUCCESS; }

ErrorCode StartQuery(QueryID query_id, const char* query_str, MatchType match_type,
					 unsigned int match_dist)
{
	return impl.addQuery(query_id, query_str, match_type, match_dist);
}

ErrorCode EndQuery(QueryID query_id) { return impl.removeQuery(query_id); }

ErrorCode MatchDocument(DocID doc_id, const char* doc_str)
{
	if(doc_id == 233){
		auto x =1;
	}
	return impl.matchDocument(doc_id, doc_str);
}

ErrorCode GetNextAvailRes(DocID* p_doc_id, unsigned int* p_num_res, QueryID** p_query_ids)
{
	return impl.getNext(p_doc_id, p_num_res, p_query_ids);;
}
