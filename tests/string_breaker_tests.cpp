#include "../lib/include/catch2/catch.hpp"

#include "../src/string_breaker.h"

TEST_CASE("String breaker full string", "[string_breaker_full_string]")
{
	const char str[] = {"first word then another word s0m3 w0rd5 *9_=@@]} a s"};
	bud::vector<bud::string> output = string_breaker(str);
	REQUIRE(output.size() == 10);
	REQUIRE(output.at(0) == "first");
	REQUIRE(output.at(1) == "word");
	REQUIRE(output.at(2) == "then");
	REQUIRE(output.at(3) == "another");
	REQUIRE(output.at(4) == "word");
	REQUIRE(output.at(5) == "s0m3");
	REQUIRE(output.at(6) == "w0rd5");
	REQUIRE(output.at(7) == "*9_=@@]}");
	REQUIRE(output.at(8) == "a");
	REQUIRE(output.at(9) == "s");
}

TEST_CASE("String breaker empty string", "[string_breaker_empty_string]")
{
	const char str[] = {""};
	bud::vector<bud::string> output = string_breaker(str);
	REQUIRE(output.size() == 0);
}
