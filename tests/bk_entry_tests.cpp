#include "../lib/include/catch2/catch.hpp"

#include "../src/BK_Entry.h"
#include "../src/editDistance.h"

static int distance(Entry *a, Entry *b)
{
	REQUIRE(a != NULL);
	REQUIRE(b != NULL);
	return int(getEdit(a->first, b->first));
}

TEST_CASE("BK Entry get", "[bk_entry_get]")
{
	Entry a("hello", bud::vector<int>());
	BK_Entry b(&distance);
	b.insert(&a);

	REQUIRE(&a == b.get("hello"));
}
