#include "../lib/include/catch2/catch.hpp"

#include "../src/string_breaker.h"

TEST_CASE("String breaker full string", "[string_breaker_full_string]")
{
	const char str[] = {"first word then another word s0m3 w0rd5 *9_=@@]} a s"};
	bud::unordered_set<bud::string> output = string_breaker(str);
	REQUIRE(output.size() == 9);
}

TEST_CASE("String breaker empty string", "[string_breaker_empty_string]")
{
	const char str[] = {""};
	bud::unordered_set<bud::string> output = string_breaker(str);
	REQUIRE(output.size() == 0);
}
