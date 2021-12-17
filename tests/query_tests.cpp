#include "../lib/include/catch2/catch.hpp"

#include <iostream>
#include "../include/core.h"
#include "../src/query.h"

TEST_CASE("Query constructor", "[query_constructor]")
{
	const char str[] = "first word then another word s0m3 w0rd5 *9_=@@]} a s";
	Query q(1, str, MatchType::MT_EXACT_MATCH, 2);
	REQUIRE(q.m_str.size() == 9);
}
