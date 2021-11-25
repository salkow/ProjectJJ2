#include "../lib/include/catch2/catch.hpp"

#include "../src/my_string.h"
#include "../src/bkTrees/appMatching/editDistance.h"

using bud::string;

TEST_CASE("Edit distance test num1", "[edit_tests1]")
{
	string a("hell");
	string b("help");
	REQUIRE(getEdit(a, b) == 1);
}

TEST_CASE("Edit distance test num2", "[edit_tests2]")
{
	string a("hello");
	string b("ooohellottt");
	REQUIRE(getEdit(a, b) == 6);
}

TEST_CASE("Edit distance test num3", "[edit_tests3]")
{
	string a("ooohellottt");
	string b("hello");
	REQUIRE(getEdit(a, b) == 6);
}
