#include "../lib/include/catch2/catch.hpp"

#include "../src/BK_T.h"
#include "../src/my_string.h"
#include "../src/vector.h"
#include "../src/appMatching/editDistance.h"

static int distance(bud::string *a, bud::string *b, unsigned int tolerance)
{
	REQUIRE(a != NULL);
	REQUIRE(b != NULL);
	return int(getEdit(*a, *b, tolerance));
}

TEST_CASE("Insert some words on a BK tree2", "[BK_insert2]")
{
	BK<bud::string> tree(&distance);
	bud::string words[7] = {"hell", "help", "fall", "felt", "fell", "small", "melt"};
	for (int i = 0; i < 7; i++)
	{
		tree.insert(&(words[i]));
	}
}

// TEST_CASE("Search some words on a BK tree", "[BK_search]")
// {
// 	BK<bud::string> tree(&distance);
// 	bud::string words[7] = {"hell", "help", "fall", "felt", "fell", "small", "melt"};
// 	for (int i = 0; i < 7; i++)
// 	{
// 		tree.insert(&(words[i]));
// 	}

// 	bud::vector<bud::string *> results;

// 	bud::string query1 = "hell";
// 	results = tree.search(&query1, 0);
// 	INFO("result is: " << *(results.back()) << " result should be: " << query1);
// 	REQUIRE(results.back() == &(words[0]));

// 	bud::string query2 = "hel1";
// 	results = tree.search(&query2, 1);
// 	REQUIRE(results.size() == 2);

// 	bud::string query3 = "el";
// 	results = tree.search(&query3, 2);
// 	REQUIRE(results.size() == 5);
// }