#include "../lib/include/catch2/catch.hpp"

#include <iostream>
#include "../src/implementation.h"

TEST_CASE("Query constructor", "[query_constructor]")
{
	const char str[MAX_DOC_LENGTH] = {"first word then another word s0m3 w0rd5 *9_=@@]} a s"};
	Query q(1, str, MatchType::MT_EXACT_MATCH, 2);
	REQUIRE(q.m_str.size() == 9);
}

TEST_CASE("Implementation Hamming", "[impl_hamming]")
{
	implementation imp;
	const char str[] = "first word then another word";
	const char str2[] = "asd word then another word";
	imp.addQuery(1, str, MT_HAMMING_DIST, 3);
	imp.addQuery(2, str2, MT_HAMMING_DIST, 2);

	REQUIRE(imp.matchDocument(2, "this first test has asd word and then it also has another") == EC_SUCCESS);

	imp.removeQuery(2);
	imp.removeQuery(1);

	REQUIRE(imp.matchDocument(5, "this first test has asd word and then it also has another") == EC_SUCCESS);

	DocID p_doc_id = 0;
	unsigned int p_num_res = 0;
	QueryID *p_query_ids = NULL;
	REQUIRE(imp.getNext(&p_doc_id, &p_num_res, &p_query_ids) == EC_SUCCESS);

	REQUIRE(p_doc_id == 5);
	REQUIRE(p_num_res == 0);

	REQUIRE(imp.getNext(&p_doc_id, &p_num_res, &p_query_ids) == EC_SUCCESS);

	REQUIRE(p_doc_id == 2);
	REQUIRE(p_num_res == 2);
	REQUIRE(p_query_ids[0] == 1);
	REQUIRE(p_query_ids[1] == 2);
}

TEST_CASE("Implementation Edit", "[impl_edit]")
{
	implementation imp;
	const char str[] = "first word then another word";
	const char str2[] = "asd word then another word";
	imp.addQuery(1, str, MT_EDIT_DIST, 3);
	imp.addQuery(2, str2, MT_EDIT_DIST, 2);

	REQUIRE(imp.matchDocument(2, "this first test has asd word and then it also has another") == EC_SUCCESS);

	imp.removeQuery(2);
	imp.removeQuery(1);

	REQUIRE(imp.matchDocument(5, "this first test has asd word and then it also has another") == EC_SUCCESS);

	DocID p_doc_id = 0;
	unsigned int p_num_res = 0;
	QueryID *p_query_ids = NULL;
	REQUIRE(imp.getNext(&p_doc_id, &p_num_res, &p_query_ids) == EC_SUCCESS);

	REQUIRE(p_doc_id == 5);
	REQUIRE(p_num_res == 0);

	REQUIRE(imp.getNext(&p_doc_id, &p_num_res, &p_query_ids) == EC_SUCCESS);

	REQUIRE(p_doc_id == 2);
	REQUIRE(p_num_res == 2);
	REQUIRE(p_query_ids[0] == 1);
	REQUIRE(p_query_ids[1] == 2);
}

TEST_CASE("Implementation Exact", "[impl_exact]")
{
	implementation imp;
	const char str[] = "first word then another word";
	const char str2[] = "asd word then another word";
	imp.addQuery(1, str, MT_EXACT_MATCH, 3);
	imp.addQuery(2, str2, MT_EXACT_MATCH, 2);

	REQUIRE(imp.matchDocument(2, "this first test has asd word and then it also has another") == EC_SUCCESS);

	imp.removeQuery(2);
	imp.removeQuery(1);

	REQUIRE(imp.matchDocument(5, "this first test has asd word and then it also has another") == EC_SUCCESS);

	DocID p_doc_id = 0;
	unsigned int p_num_res = 0;
	QueryID *p_query_ids = NULL;
	REQUIRE(imp.getNext(&p_doc_id, &p_num_res, &p_query_ids) == EC_SUCCESS);

	REQUIRE(p_doc_id == 5);
	REQUIRE(p_num_res == 0);

	REQUIRE(imp.getNext(&p_doc_id, &p_num_res, &p_query_ids) == EC_SUCCESS);

	REQUIRE(p_doc_id == 2);
	REQUIRE(p_num_res == 2);
	REQUIRE(p_query_ids[0] == 1);
	REQUIRE(p_query_ids[1] == 2);
}
