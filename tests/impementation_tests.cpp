#include "../lib/include/catch2/catch.hpp"

#include <iostream>
#include "../src/implementation.h"

TEST_CASE("Query constructor", "[query_constructor]")
{
	const char str[MAX_DOC_LENGTH] = {"first word then another word s0m3 w0rd5 *9_=@@]} a s"};
	Query q(1, str, MatchType::MT_EXACT_MATCH, 2);
	REQUIRE(q.m_str.size() == 9);
}
