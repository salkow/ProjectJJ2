#include "../lib/include/catch2/catch.hpp"

#include <iostream>
#include "../src/implementation.h"

TEST_CASE("Query constructor", "[query_constructor]")
{
	const char str[MAX_DOC_LENGTH] = {"first word then another word s0m3 w0rd5 *9_=@@]} a s"};
	Query q(1, str, MatchType::MT_EXACT_MATCH, 2);
	REQUIRE(q.m_str.size() == 9);
}

TEST_CASE("hammming", "[hamming__239829]")
{
	implementation imp;
	const char str[] = "first word then another word";
	const char str2[] = "asd word then another word";
	imp.addQuery(1, str, MT_HAMMING_DIST, 3);
	imp.addQuery(2, str2, MT_HAMMING_DIST, 2);

	imp.removeQuery(2);
	imp.removeQuery(1);
}

TEST_CASE("eddito", "[edit_0a9sd]")
{
	implementation imp;
	const char str[] = "first word then another word";
	const char str2[] = "asd word then another word";
	imp.addQuery(1, str, MT_EDIT_DIST, 3);
	imp.addQuery(2, str2, MT_EDIT_DIST, 2);

	imp.removeQuery(2);
	imp.removeQuery(1);
}
