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
	Entry a("hello", bud::vector<int>());
	BK_Entry b(&distance);
	b.insert(&a);

	REQUIRE(b.get("hello") == &a);

	REQUIRE(b.get("this does not exist in the BK") == NULL);
}
