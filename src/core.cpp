#include "../include/core.h"
#include "implementation.h"

namespace
{
    implementation impl;    
}

ErrorCode InitializeIndex()
{
    impl.add_to_vec(5);

    return EC_SUCCESS;
}

ErrorCode DestroyIndex()
{
    return EC_SUCCESS;
}

ErrorCode StartQuery(QueryID        query_id,   
                     const char*    query_str,
                     MatchType      match_type,
                     unsigned int   match_dist)
{
    return EC_SUCCESS;
}

ErrorCode EndQuery(QueryID query_id)
{
    return EC_SUCCESS;
}

ErrorCode MatchDocument(DocID         doc_id,
                        const char*   doc_str)
{
    return EC_SUCCESS;
}

ErrorCode GetNextAvailRes(DocID*         p_doc_id,
                          unsigned int*  p_num_res,
                          QueryID**      p_query_ids)
{
    return EC_SUCCESS;
}

