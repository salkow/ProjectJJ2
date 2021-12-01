#ifndef IMPL_H
#define IMPL_H

#include "bkTree.h"
// #include "../include/core.h"
// #include "my_string.h"
#include "unique_ptr.h"
#include "unordered_map.h"
#include "util.h"

class implementation
{
public:
    implementation() = default;
    ~implementation() = default;
    ErrorCode addQuery(QueryID id, const char* str, MatchType match_type, unsigned int tolerance){
        auto* query = new Query(id, str, match_type, tolerance);
        if(match_type == MT_EDIT_DIST){
            auto result = m_queries_ht->insert(bud::pair<const QueryID, Query*>(id, query));
            if (!result.second)
                return EC_FAIL;
            m_edit_bk->add(query, tolerance);
        }
        return EC_SUCCESS;
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

    bud::unique_ptr<bkTree> m_edit_bk = bud::make_unique<bkTree>(new bkTree(MatchType::MT_EDIT_DIST));

};

#endif // IMPL_H
