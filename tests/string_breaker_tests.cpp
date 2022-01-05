#include "../lib/include/catch2/catch.hpp"

#include "../src/string_breaker.h"

TEST_CASE("String breaker full string", "[string_breaker_full_string]")
{
	const char str[] = "first word then another word s0m3 w0rd5 *9_=@@]} a s";
	bud::vector<bud::string> output = string_breaker(str);
	REQUIRE(output.size() == 9);

//	REQUIRE(output["first"]);
//	REQUIRE(output["word"]);
//	REQUIRE(output["then"]);
//	REQUIRE(output["another"]);
//	REQUIRE(output["s0m3"]);
//	REQUIRE(output["w0rd5"]);
//	REQUIRE(output["first"]);
//	REQUIRE(output["*9_=@@]}"]);
//	REQUIRE(output["a"]);
//	REQUIRE(output["s"]);
}

TEST_CASE("String breaker empty string", "[string_breaker_empty_string]")
{
	const char str[] = {""};
	bud::vector<bud::string> output = string_breaker(str);
	REQUIRE(output.size() == 0);
}
