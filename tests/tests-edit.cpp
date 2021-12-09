#include "../lib/include/catch2/catch.hpp"

#include "../src/implementation.h"
#include "../src/util.h"
TEST_CASE("test", "[test]")
{
	implementation impl;
	auto result = impl.addQuery(1, "tesa is a test", MT_EDIT_DIST, 0);
	bud::string t = "test";
	bud::string p = "tesa";
	auto vec = impl.m_edit_bk->search(&t, 1);
	//	REQUIRE(vec.size() == 0);
	// TODO: Why 0^
	//	REQUIRE(bud::find_if(vec.begin(), vec.end(), [&](bud::string* str1) { return *str1 == t; })
	//!= 			vec.end()); 	REQUIRE(bud::find_if(vec.begin(), vec.end(), [&](bud::string* str1) {
	//!return
	//*str1 == p; }) != 			vec.end());
	//	REQUIRE(result == EC_SUCCESS);
}
