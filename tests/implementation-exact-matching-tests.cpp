#include "../lib/include/catch2/catch.hpp"

#include "../src/implementation.h"
#include "../src/array.h"

TEST_CASE("No match exact matching queries", "[no_match_exact_matching_queries]")
{
	implementation impl;

	REQUIRE(impl.addQuery(0, "this is a query", MT_EXACT_MATCH, 1) == EC_SUCCESS);
	REQUIRE(impl.addQuery(1, "this is another a query", MT_EXACT_MATCH, 1) == EC_SUCCESS);
	REQUIRE(impl.addQuery(2, "this is a third query", MT_EXACT_MATCH, 1) == EC_SUCCESS);
	REQUIRE(impl.addQuery(3, "this is the last query", MT_EXACT_MATCH, 1) == EC_SUCCESS);

	REQUIRE(impl.matchDocument(0, "the quick fox jumps over the lazy dog") == EC_SUCCESS);

	DocID p_doc_id;
	unsigned int p_num_res;
	QueryID* p_query_ids;

	REQUIRE(impl.getNext(&p_doc_id, &p_num_res, &p_query_ids) == EC_SUCCESS);

	REQUIRE(p_num_res == 0);

	free(p_query_ids);
}

TEST_CASE("Match exact matching queries", "[match_exact_matching_queries]")
{
	implementation impl;

	REQUIRE(impl.addQuery(0, "this is a query", MT_EXACT_MATCH, 1) == EC_SUCCESS);
	REQUIRE(impl.addQuery(1, "this is another a query", MT_EXACT_MATCH, 1) == EC_SUCCESS);
	REQUIRE(impl.addQuery(2, "this is a third query", MT_EXACT_MATCH, 1) == EC_SUCCESS);
	REQUIRE(impl.addQuery(3, "this is the last query", MT_EXACT_MATCH, 1) == EC_SUCCESS);

	REQUIRE(impl.matchDocument(0, "this is a query third last") == EC_SUCCESS);

	DocID p_doc_id;
	unsigned int p_num_res;
	QueryID* p_query_ids;

	REQUIRE(impl.getNext(&p_doc_id, &p_num_res, &p_query_ids) == EC_SUCCESS);

	QueryID* p_query_ids_end = p_query_ids + p_num_res;

	REQUIRE(p_num_res == 2);

	REQUIRE(bud::find(p_query_ids, p_query_ids_end, 0U) != p_query_ids_end);
	REQUIRE(bud::find(p_query_ids, p_query_ids_end, 2U) != p_query_ids_end);

	free(p_query_ids);
}

TEST_CASE("Delete exact matching queries", "[delete_exact_matching_queries]")
{
	implementation impl;

	REQUIRE(impl.addQuery(0, "this is a query", MT_EXACT_MATCH, 1) == EC_SUCCESS);
	REQUIRE(impl.addQuery(1, "this is another a query", MT_EXACT_MATCH, 1) == EC_SUCCESS);
	REQUIRE(impl.addQuery(2, "this is a third query", MT_EXACT_MATCH, 1) == EC_SUCCESS);
	REQUIRE(impl.addQuery(3, "this is the last query", MT_EXACT_MATCH, 1) == EC_SUCCESS);

	REQUIRE(impl.removeQuery(0) == EC_SUCCESS);

	REQUIRE(impl.matchDocument(0, "this is a query third last") == EC_SUCCESS);

	DocID p_doc_id;
	unsigned int p_num_res;
	QueryID* p_query_ids;

	REQUIRE(impl.getNext(&p_doc_id, &p_num_res, &p_query_ids) == EC_SUCCESS);

	QueryID* p_query_ids_end = p_query_ids + p_num_res;

	REQUIRE(p_num_res == 1);

	REQUIRE(bud::find(p_query_ids, p_query_ids_end, 2U) != p_query_ids_end);

	free(p_query_ids);
}

TEST_CASE("Match multiple documents exact matching", "[match_multiple_documents_exact_matching]")
{
	implementation impl;

	REQUIRE(impl.addQuery(0, "this is a query", MT_EXACT_MATCH, 1) == EC_SUCCESS);
	REQUIRE(impl.addQuery(1, "this is another a query", MT_EXACT_MATCH, 1) == EC_SUCCESS);
	REQUIRE(impl.addQuery(2, "this is a third query", MT_EXACT_MATCH, 1) == EC_SUCCESS);
	REQUIRE(impl.addQuery(3, "this is the last query", MT_EXACT_MATCH, 1) == EC_SUCCESS);

	REQUIRE(impl.matchDocument(0, "this is a query third last") == EC_SUCCESS);

	REQUIRE(impl.removeQuery(0) == EC_SUCCESS);

	REQUIRE(impl.matchDocument(1, "this is a another query") == EC_SUCCESS);

	DocID p_doc_id;
	unsigned int p_num_res;
	QueryID* p_query_ids;

	for (int i = 0; i < 2; i++)
	{
		REQUIRE(impl.getNext(&p_doc_id, &p_num_res, &p_query_ids) == EC_SUCCESS);

		if (p_doc_id == 0)
		{
			REQUIRE(p_num_res == 2);

			QueryID* p_query_ids_end = p_query_ids + p_num_res;

			REQUIRE(bud::find(p_query_ids, p_query_ids_end, 2U) != p_query_ids_end);
			REQUIRE(bud::find(p_query_ids, p_query_ids_end, 2U) != p_query_ids_end);

			free(p_query_ids);
		}

		else if (p_doc_id == 1)
		{
			REQUIRE(p_num_res == 1);

			QueryID* p_query_ids_end = p_query_ids + p_num_res;

			REQUIRE(bud::find(p_query_ids, p_query_ids_end, 1U) != p_query_ids_end);

			free(p_query_ids);
		}

		else
			REQUIRE(false);
	}
}

TEST_CASE("Implementation Exact", "[impl_exact]")
{
	implementation imp;
	const char str[] = "first word then another word";
	const char str2[] = "asd word then another word";
	imp.addQuery(1, str, MT_EXACT_MATCH, 3);
	imp.addQuery(2, str2, MT_EXACT_MATCH, 2);

	REQUIRE(imp.matchDocument(2, "this first test has asd word and then it also has another") ==
			EC_SUCCESS);

	imp.removeQuery(2);
	imp.removeQuery(1);

	REQUIRE(imp.matchDocument(5, "this first test has asd word and then it also has another") ==
			EC_SUCCESS);

	DocID p_doc_id = 0;
	unsigned int p_num_res = 0;
	QueryID* p_query_ids = nullptr;
	REQUIRE(imp.getNext(&p_doc_id, &p_num_res, &p_query_ids) == EC_SUCCESS);

	free(p_query_ids);

	REQUIRE(p_doc_id == 5);
	REQUIRE(p_num_res == 0);

	REQUIRE(imp.getNext(&p_doc_id, &p_num_res, &p_query_ids) == EC_SUCCESS);

	REQUIRE(p_doc_id == 2);
	REQUIRE(p_num_res == 2);
	REQUIRE(p_query_ids[0] == 1);
	REQUIRE(p_query_ids[1] == 2);

	free(p_query_ids);
}
