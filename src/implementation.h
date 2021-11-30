#ifndef IMPL_H
#define IMPL_H

#include "vector.h"
#include "../include/core.h"
#include "my_string.h"
#include "unordered_map.h"
#include "util.h"

struct Query {
    QueryID                     m_id;
    bud::vector<bud::string>    m_str;
    MatchType                   m_match_type;
    unsigned int                m_tolerance;
};

struct Result{
    DocID         m_doc_id;
    unsigned int  m_num_res;
    QueryID*      m_query_ids;
};


class implementation
{
public:
    implementation() = default;
    ~implementation() = default;
    ErrorCode addQuery(QueryID id, const char* str, MatchType match_type, unsigned int tolerance){
                
    };
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

    

private:
    bud::unordered_map<bud::string, bud::vector<Query*>, HashFunction> *m_words_ht;
    bud::unordered_map<QueryID, Query*, HashFunction> *m_queries_ht;
    bud::vector<Result> m_res;
};

#endif // IMPL_H
