#include "../lib/include/catch2/catch.hpp"

#include "../src/BK_T.h"
#include "../src/my_string.h"
#include "../src/vector.h"
#include "../src/appMatching/editDistance.h"

static int distance(bud::string *a, bud::string *b, unsigned int tolerance)
{
	REQUIRE(a != NULL);
	REQUIRE(b != NULL);
	if (tolerance == 0)
	{
		return int(getEdit(*a, *b, MAX_WORD_LENGTH+1));
	}
	else
		return int(getEdit(*a, *b, tolerance));
}

TEST_CASE("Insert some words on a BK tree", "[BK_insert]")
{
	BK<bud::string> tree(&distance);
	bud::string words[7] = {"hell", "help", "fall", "felt", "fell", "small", "melt"};
	for (int i = 0; i < 7; i++)
	{
		tree.insert(new bud::string(words[i]));
	}
}

TEST_CASE("Search some words on a BK tree", "[BK_search]")
{
	BK<bud::string> tree(&distance);
	bud::string words[7] = {"hell", "help", "fall", "felt", "fell", "small", "melt"};
	bud::vector<bud::string *> w;
	for (int i = 0; i < 7; i++)
	{
		bud::string *tmp = new bud::string(words[i]);
		w.push_back(tmp);
		tree.insert(tmp);
	}

	bud::vector<bud::string *> results;

	bud::string query1 = "hell";
	results = tree.search(&query1, 0);
	//	std::cout << results.size() << std::endl;
	INFO("result is: " << *(results.back()) << " result should be: " << query1);
	REQUIRE(results.back() == w[0]);

	bud::string query2 = "hel1";
	results = tree.search(&query2, 1);
	REQUIRE(results.size() == 2);

	bud::string query3 = "el";
	results = tree.search(&query3, 2);
	REQUIRE(results.size() == 5);
}

TEST_CASE("Remove a node", "[BK_remove]")
{
	BK<bud::string> tree(&distance);
	bud::string words[7] = {"hell", "help", "fall", "felt", "fell", "small", "melt"};
	bud::vector<bud::string *> w;
	for (int i = 0; i < 7; i++)
	{
		bud::string *tmp = new bud::string(words[i]);
		w.push_back(tmp);
		tree.insert(tmp);
	}

	tree.remove(w[1]);
	bud::string query2 = "hel1";
	bud::vector<bud::string *> results = tree.search(&query2, 1);
	REQUIRE(results.size() == 1);
	REQUIRE(results.at(0) == w[0]);
}

TEST_CASE("Restore a node", "[BK_restore]")
{
	BK<bud::string> tree(&distance);
	bud::string words[7] = {"hell", "help", "fall", "felt", "fell", "small", "melt"};
	bud::vector<bud::string *> w;
	for (int i = 0; i < 7; i++)
	{
		bud::string *tmp = new bud::string(words[i]);
		w.push_back(tmp);
		tree.insert(tmp);
	}

	tree.remove(w[1]);
	tree.restore(w[1]);
	bud::string query2 = "hel1";
	bud::vector<bud::string *> results = tree.search(&query2, 1);
	REQUIRE(results.size() == 2);
}
