#include "../lib/include/catch2/catch.hpp"

#include "../src/implementation.h"
#include "../src/util.h"
TEST_CASE("test", "[test]")
{
	implementation impl;
	auto result = impl.addQuery(1, "tesa is a test", MT_EDIT_DIST, 0);
	REQUIRE(result == EC_SUCCESS);
	impl.removeQuery(1);
	REQUIRE(result == EC_SUCCESS);
}

TEST_CASE("test1", "[test1]")
{
	implementation impl;
	auto result = impl.addQuery(1, "tesa is a test", MT_EXACT_MATCH, 0);
	REQUIRE(result == EC_SUCCESS);
	impl.removeQuery(1);
	REQUIRE(result == EC_SUCCESS);
}
