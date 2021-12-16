#include "../lib/include/catch2/catch.hpp"
#include "../src/appMatching/editDistance.h"
#include "../src/appMatching/hammingDistance.h"

TEST_CASE("edit_distance_test", "[edit_distance_test]")
{
	bud::string a = "1234  34";
	bud::string b = "0000";
	REQUIRE(getEdit(a, b, 0) == 8);
	REQUIRE(getEdit(a, b, 2) == 2);
}

TEST_CASE("hamming_distance_test", "[hamming_distance_test]")
{
	bud::string a = "1234  34";
	bud::string b = "0000____";
	REQUIRE(getHamming(a, b, 0) == 8);
	REQUIRE(getHamming(a, b, 2) == 2);
}
