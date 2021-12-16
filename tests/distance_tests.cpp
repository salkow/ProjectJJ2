#include "../lib/include/catch2/catch.hpp"
#include "../src/appMatching/editDistance.h"
#include "../src/appMatching/hammingDistance.h"

TEST_CASE("edit_distance_test", "[edit_distance_test]")
{
	bud::string a = "hell";
	bud::string b = "fell";
	REQUIRE(getEdit(b, a, 2) == 1);
	// REQUIRE(getEdit(a, b, 2) == 2);
}

TEST_CASE("hamming_distance_test", "[hamming_distance_test]")
{
	return;
	bud::string a = "hell___";
	bud::string b = "fell111";
	REQUIRE(getHamming(b, a, 2) == 2);
	// REQUIRE(getHamming(a, b, 0) == 8);
	// REQUIRE(getHamming(a, b, 2) == 2);
}
