#include "../lib/include/catch2/catch.hpp"

#include "../src/BK_Entry.h"
#include "../src/appMatching/editDistance.h"

static int distance(Entry *a, Entry *b, unsigned int max_t)
{
	REQUIRE(a != NULL);
	REQUIRE(b != NULL);
	return int(getEdit(a->first, b->first, max_t));
}

TEST_CASE("BK Entry get", "[bk_entry_get]")
{
	Entry a("hello", bud::unordered_set<Query *>());
	Entry b("heyy", bud::unordered_set<Query *>());
	Entry c("hola", bud::unordered_set<Query *>());
	BK_Entry tree(&distance);
	//insert not in order
	tree.insert(&b);
	tree.insert(&a);
	tree.insert(&c);

	REQUIRE(tree.get("hello") == &a);
	REQUIRE(tree.get("heyy") == &b);
	REQUIRE(tree.get("hola") == &c);


	REQUIRE(tree.get("this does not exist in the BK") == NULL);
}
