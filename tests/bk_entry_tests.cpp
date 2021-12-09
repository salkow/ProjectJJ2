#include "../lib/include/catch2/catch.hpp"

#include "../src/BK_Entry.h"
#include "../src/appMatching/editDistance.h"
#include "../src/unique_ptr.h"

static int distance(Entry *a, Entry *b, unsigned int max_t)
{
	REQUIRE(a != NULL);
	REQUIRE(b != NULL);
	return int(getEdit(a->first, b->first, max_t));
}

TEST_CASE("BK Entry get from empty", "[bk_entry_get_empty]")
{
	BK_Entry b(&distance);

	REQUIRE(b.get("hello") == NULL);
}

TEST_CASE("BK Entry get", "[bk_entry_get]")
{
	Entry a("hello", bud::unordered_set<Query *>());
	BK_Entry b(&distance);
	b.insert(&a);

	REQUIRE(b.get("hello") == &a);

	REQUIRE(b.get("this does not exist in the BK") == NULL);
}

TEST_CASE("BK Entry two insert", "[bk_entry_two_insert]")
{
	Entry a("hello", bud::unordered_set<Query *>());
	Entry ap("hellop", bud::unordered_set<Query *>());
	BK_Entry b(&distance);
	b.insert(&a);

	REQUIRE(b.get("hello") == &a);
	b.insert(&ap);
	REQUIRE(b.get("hellop") == &ap);
	REQUIRE(b.get("this does not exist in the BK") == NULL);
}