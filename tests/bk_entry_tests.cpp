#include "../lib/include/catch2/catch.hpp"

#include "../src/BK_Entry.h"
#include "../src/appMatching/editDistance.h"
#include "../src/unique_ptr.h"

static int distance(Entry *a, Entry *b, unsigned int max_t)
{
	REQUIRE(a != NULL);
	REQUIRE(b != NULL);
	if (max_t == 0)
	{
		return int(getEdit(a->first, b->first, 1000));
	}
	else
	{
		return int(getEdit(a->first, b->first, max_t));
	}
}

TEST_CASE("BK Entry get", "[bk_entry_get]")
{
	Entry *a = new Entry("hello", bud::unordered_set<Query *>());
	Entry *b = new Entry("heyy", bud::unordered_set<Query *>());
	Entry *c = new Entry("hola", bud::unordered_set<Query *>());
	BK_Entry tree(&distance);
	//insert not in order
	tree.insert(b);
	tree.insert(a);
	tree.insert(c);

	REQUIRE(tree.get("hello") == a);
	REQUIRE(tree.get("heyy") == b);
	REQUIRE(tree.get("hola") == c);

	REQUIRE(tree.get("this does not exist in the BK") == NULL);
}

TEST_CASE("BK Entry search", "[bk_entry_search]")
{
	bud::string words[7] = {"hell", "help", "fall", "felt", "fell", "small", "melt"};
	bud::vector<Entry *> entries;
	BK_Entry tree(&distance);

	//create entries and add to tree
	for (int i = 0; i < 7; i++)
	{
		Entry *entry = new Entry(words[i], bud::unordered_set<Query *>());
		entries.push_back(entry);
		tree.insert(entry);
	}

	bud::vector<bud::pair<Entry *, int>> results = tree.search("hell", 0);

	REQUIRE(results.size() == 1);
	REQUIRE(results.at(0).first == entries.at(0));

	results = tree.search("hell", 1);

	std::cout << results[0].first->first << std::endl;
	std::cout << results[1].first->first << std::endl;

	REQUIRE(results.size() == 3);

	REQUIRE(results.at(0).first == entries.at(0)); //hell
	REQUIRE(results.at(0).second == 0);
	REQUIRE(results.at(1).first == entries.at(1)); //help
	REQUIRE(results.at(1).second == 1);
	REQUIRE(results.at(2).first == entries.at(4)); //fell
	REQUIRE(results.at(2).second == 1);

	results = tree.search("hell", 2);

	REQUIRE(results.size() == 6);
	REQUIRE(results.at(0).first == entries.at(0)); //hell
	REQUIRE(results.at(0).second == 0);
	REQUIRE(results.at(1).first == entries.at(1)); //help
	REQUIRE(results.at(1).second == 1);
	REQUIRE(results.at(2).first == entries.at(4)); //fell
	REQUIRE(results.at(2).second == 1);
	REQUIRE(results.at(3).first == entries.at(2)); //fall
	REQUIRE(results.at(3).second == 2);
	REQUIRE(results.at(4).first == entries.at(3)); //felt
	REQUIRE(results.at(4).second == 2);
	REQUIRE(results.at(5).first == entries.at(6)); //melt
	REQUIRE(results.at(5).second == 2);
}
