#include "../lib/include/catch2/catch.hpp"

#include "../src/implementation.h"

TEST_CASE("Add exact matching queries", "[add_exact_matching_queries]")
{
	implementation impl;

	impl.addQuery(0, "this is a query", MT_EXACT_MATCH, 1);
}
