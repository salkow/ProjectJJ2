#include "implementation.h"



implementation() = default;
    ~implementation() = default;
    ErrorCode addQuery(QueryID id, const char* str, MatchType match_type, unsigned int tolerance){};
    ErrorCode removeQuery(QueryID id){};
    ErrorCode getNext(DocID* p_doc_id, unsigned int* p_num_res, QueryID** p_query_ids){
        if(m_res.size() ==0){
            return EC_NO_AVAIL_RES;
        }
        *p_doc_id = m_res.back().m_doc_id;
        *p_num_res = m_res.back().m_num_res;
        *p_query_ids = m_res.back().m_query_ids;

        m_res.pop_back();
        return EC_SUCCESS;
    }