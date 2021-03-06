#include "../lib/include/catch2/catch.hpp"

#include "../src/forward_list.h"

using bud::forward_list;

TEST_CASE("Check if a new forward_list is empty.", "[empty_forward_list]")
{
	forward_list<int> empty_forward_list;
	REQUIRE(empty_forward_list.empty());
}

TEST_CASE("Add item to empty forward list.", "[add_item_to_empty_forward_list]")
{
	forward_list<int> list;

	list.emplace_front(9);

	REQUIRE(list.front() == 9);

	REQUIRE(list.empty() == false);
}

TEST_CASE("Add many items to forward list.", "[add_many_items_to_forward_list]")
{
	forward_list<int> list;

	list.push_front(19);
	list.push_front(29);
	list.emplace_front(39);
	list.emplace_front(49);

	auto curr_value = list.begin();

	REQUIRE(*curr_value == 49);
	++curr_value;

	REQUIRE(*curr_value == 39);
	++curr_value;

	REQUIRE(*curr_value == 29);
	++curr_value;

	REQUIRE(*curr_value == 19);
	++curr_value;

	REQUIRE(curr_value == list.end());

	REQUIRE(list.empty() == false);
}

TEST_CASE("Test iterators to forward_list.", "[test_iterators_forward_list]")
{
	int sum = 0;

	forward_list<int> my_list;

	my_list.push_front(1);
	my_list.push_front(2);
	my_list.emplace_front(3);
	my_list.emplace_front(4);
	my_list.push_front(5);
	my_list.emplace_front(6);

	for (const auto& x : my_list)
		sum += x;

	REQUIRE(sum == 21);

	forward_list<int> my_other_list;

	for (const auto& x : my_other_list)
		sum += x;

	REQUIRE(sum == 21);
}
