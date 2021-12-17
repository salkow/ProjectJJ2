#include "../lib/include/catch2/catch.hpp"
#include "../src/appMatching/hammingDistance.h"

TEST_CASE("hamming_distance_test", "[hamming_distance_test]")
{
	bud::string a = "hell";
	bud::string b = "fell";
	REQUIRE(getHamming(b, a, MAX_WORD_LENGTH) == 1);
	// REQUIRE(getEdit(a, b, 2) == 2);
}

TEST_CASE("hamming_distance_test2", "[hamming_distance_test2]")
{
	bud::string a = "hell";
	bud::string b = "fell";
	REQUIRE(getHamming(a, b, MAX_WORD_LENGTH) == 1);
}

TEST_CASE("hamming_distance_test3", "[hamming_distance_test3]")
{
	bud::string a = "abcd";
	bud::string b = "efgh";
	REQUIRE(getHamming(b, a, MAX_WORD_LENGTH) == 4);
}

TEST_CASE("hamming_distance_test4", "[hamming_distance_test4]")
{
	bud::string a = "abcd";
	bud::string b = "efgh";
	REQUIRE(getHamming(b, a, 2) == 2);
}