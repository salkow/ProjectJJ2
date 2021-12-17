#include "../lib/include/catch2/catch.hpp"

#include "../src/my_string.h"
#include "../src/appMatching/editDistance.h"
#include "../src/unordered_set.h"
using bud::string;

TEST_CASE("Edit distance test num1", "[edit_tests1]")
{
	string a("hell");
	string b("help");
	REQUIRE(getEdit(a, b, MAX_WORD_LENGTH) == 1);
}

TEST_CASE("Edit distance test num2", "[edit_tests2]")
{
	string a("hello");
	string b("ooohellottt");
	REQUIRE(getEdit(a, b, MAX_WORD_LENGTH) == 6);
}

TEST_CASE("Edit distance test num3", "[edit_tests3]")
{
	string a("ooohellottt");
	string b("hello");
	REQUIRE(getEdit(a, b, MAX_WORD_LENGTH) == 6);
}

TEST_CASE("Edit distance test num4", "[edit_tests4]")
{
	string a("hello");
	string b("ooohellottt");
	REQUIRE(getEdit(a, b, 4) == 4);
}

TEST_CASE("Edit distance test num5", "[edit_tests5]")
{
	Entry a("ooohellottt", bud::unordered_set<Query*>());
	Entry b("hello", bud::unordered_set<Query*>());
	REQUIRE(editDistance(&a, &b,4) == 4);
}
