#include "../lib/include/catch2/catch.hpp"

#include "../src/unordered_map.h"
#include "../src/my_string.h"

using bud::pair;
using bud::string;
using bud::unordered_map;

TEST_CASE("Search for items in empty map.", "[search_in_empty_map]")
{
	unordered_map<int, char> my_map(2);

	REQUIRE(my_map[2345] == nullptr);
	REQUIRE(my_map[93] == nullptr);
	REQUIRE(my_map[0] == nullptr);
	REQUIRE(my_map[231] == nullptr);
	REQUIRE(my_map[2] == nullptr);
	REQUIRE(my_map[3] == nullptr);

	REQUIRE(my_map.empty());
}

TEST_CASE("Insert items to unordered_map.", "[insert_to_unordered_map]")
{
	unordered_map<int, char> my_map(3);

	auto my_pair = my_map.try_emplace(1, 'a');

	REQUIRE(my_pair.second == true);

	my_map.try_emplace(2, 'b');
	my_map.try_emplace(3, 'c');
	my_map.try_emplace(4, 'd');
	my_map.try_emplace(5, 'e');
	my_map.try_emplace(6, 'f');
	my_map.try_emplace(7, 'g');

	REQUIRE(*my_map[1] == 'a');
	REQUIRE(*my_map[2] == 'b');
	REQUIRE(*my_map[3] == 'c');
	REQUIRE(*my_map[4] == 'd');
	REQUIRE(*my_map[5] == 'e');
	REQUIRE(*my_map[6] == 'f');
	REQUIRE(*my_map[7] == 'g');

	REQUIRE(my_map.size() == 7);
}

TEST_CASE("Modify value inserted to unordered map.", "[modify_value_inserted_to_map]")
{
	unordered_map<int, char> my_map;

	my_map.try_emplace(99, 'a');
	my_map.try_emplace(0, 'b');

	*my_map[99] = 'y';
	*my_map[0] = 'z';

	REQUIRE(*my_map[99] == 'y');
	REQUIRE(*my_map[0] == 'z');

	*my_map[99] = 'o';
	REQUIRE(*my_map[99] == 'o');
}

TEST_CASE("Store many items in one unordered map list.", "[multiple_items_one_unordered_map_list]")
{
	unordered_map<int, char> my_map(1);

	my_map.try_emplace(54, 'a');
	my_map.try_emplace(99, 'b');
	my_map.try_emplace(18, 'c');
	my_map.try_emplace(60, 'd');

	REQUIRE(*my_map[54] == 'a');
	REQUIRE(*my_map[99] == 'b');
	REQUIRE(*my_map[18] == 'c');
	REQUIRE(*my_map[60] == 'd');

	REQUIRE(my_map.size() == 4);
}

TEST_CASE("Try to find non existing item in unordered_map.",
		  "[find_non_existing_item_unordered_map]")
{
	unordered_map<int, char> my_map(100);

	my_map.try_emplace(54, 'a');
	my_map.try_emplace(99, 'b');
	my_map.try_emplace(18, 'c');
	my_map.try_emplace(60, 'd');

	REQUIRE(my_map[53] == nullptr);
	REQUIRE(my_map[23] == nullptr);
	REQUIRE(my_map[0] == nullptr);
	REQUIRE(my_map[999] == nullptr);
	REQUIRE(my_map[111] == nullptr);
}

TEST_CASE("Insert existing key in unordered_map.", "[insert_existing_key_unordered_map]")
{
	unordered_map<int, char> my_map;

	auto return_value = my_map.try_emplace(54, 'a');
	REQUIRE(return_value.second == true);

	return_value = my_map.try_emplace(54, 'b');
	REQUIRE(return_value.second == false);

	REQUIRE(*my_map[54] == 'a');
}

TEST_CASE("Erase element unordered_map", "[erase_element_unordered_map]")
{
	unordered_map<int, char> my_map(7);

	REQUIRE(my_map.erase(54) == 0);
	REQUIRE(my_map.erase(99) == 0);
	REQUIRE(my_map.erase(18) == 0);
	REQUIRE(my_map.erase(60) == 0);

	my_map.try_emplace(54, 'a');
	my_map.try_emplace(99, 'b');
	my_map.try_emplace(18, 'c');
	my_map.try_emplace(60, 'd');

	REQUIRE(my_map.erase(54) == 1);
	REQUIRE(my_map.size() == 3);

	REQUIRE(*my_map[99] == 'b');
	REQUIRE(*my_map[18] == 'c');
	REQUIRE(*my_map[60] == 'd');

	REQUIRE(my_map.erase(62) == 0);
	REQUIRE(my_map.erase(61) == 0);
	REQUIRE(my_map.size() == 3);

	REQUIRE(my_map.erase(18) == 1);
	REQUIRE(my_map.size() == 2);

	REQUIRE(*my_map[99] == 'b');
	REQUIRE(*my_map[60] == 'd');

	REQUIRE(my_map.erase(60) == 1);
	REQUIRE(my_map.size() == 1);

	REQUIRE(my_map.erase(99) == 1);
	REQUIRE(my_map.size() == 0);

	REQUIRE(my_map[54] == nullptr);
	REQUIRE(my_map[99] == nullptr);
	REQUIRE(my_map[18] == nullptr);
	REQUIRE(my_map[60] == nullptr);

	REQUIRE(my_map.erase(54) == 0);
	REQUIRE(my_map.erase(99) == 0);
	REQUIRE(my_map.erase(18) == 0);
	REQUIRE(my_map.erase(60) == 0);
}

TEST_CASE("Rehash unordered_map", "[rehash_unordered_map]")
{
	unordered_map<int, char> my_map(2);

	my_map.try_emplace(2, 'b');
	my_map.try_emplace(3, 'c');
	my_map.try_emplace(4, 'd');
	my_map.try_emplace(5, 'e');
	my_map.try_emplace(6, 'f');
	my_map.try_emplace(7, 'g');

	my_map.rehash(1);
	my_map.rehash(2);
	my_map.rehash(3);
	my_map.rehash(4);
	my_map.rehash(100);

	REQUIRE(*my_map[2] == 'b');
	REQUIRE(*my_map[3] == 'c');
	REQUIRE(*my_map[4] == 'd');
	REQUIRE(*my_map[5] == 'e');
	REQUIRE(*my_map[6] == 'f');
	REQUIRE(*my_map[7] == 'g');
}

TEST_CASE("unordered_map class value", "[unordered_map_class_value]")
{
	unordered_map<int, string> my_map(2);

	my_map.try_emplace(2, "b");
	my_map.try_emplace(3, "c");
	my_map.try_emplace(4, "d");
	my_map.try_emplace(5, "e");
	my_map.try_emplace(6, "f");
	my_map.try_emplace(7, "g");

	my_map.rehash(1);
	my_map.rehash(2);
	my_map.rehash(3);
	my_map.rehash(4);
	my_map.rehash(100);

	REQUIRE(*my_map[2] == "b");
	REQUIRE(*my_map[3] == "c");
	REQUIRE(*my_map[4] == "d");
	REQUIRE(*my_map[5] == "e");
	REQUIRE(*my_map[6] == "f");
	REQUIRE(*my_map[7] == "g");
}

TEST_CASE("unordered_map class key", "[unordered_map_class_key]")
{
	unordered_map<string, int> my_map(2);

	my_map.try_emplace("b", 2);
	my_map.try_emplace("c", 3);
	my_map.try_emplace("d", 4);
	my_map.try_emplace("e", 5);
	my_map.try_emplace("f", 6);
	my_map.try_emplace("g", 7);

	my_map.rehash(1);
	my_map.rehash(2);
	my_map.rehash(3);
	my_map.rehash(4);
	my_map.rehash(100);

	REQUIRE(*my_map["b"] == 2);
	REQUIRE(*my_map["c"] == 3);
	REQUIRE(*my_map["d"] == 4);
	REQUIRE(*my_map["e"] == 5);
	REQUIRE(*my_map["f"] == 6);
	REQUIRE(*my_map["g"] == 7);
}

TEST_CASE("unordered_map iterators", "[unordered_map_iterators]")
{
	unordered_map<int, char> my_map(7);

	for (const auto& bucket : my_map.data())
		REQUIRE(bucket.begin() == bucket.end());

	my_map.try_emplace(1, '1');
	my_map.try_emplace(2, '2');
	my_map.try_emplace(3, '3');
	my_map.try_emplace(4, '4');
	my_map.try_emplace(5, '5');
	my_map.try_emplace(6, '6');

	REQUIRE(my_map.size() == 6);

	for (char i = 1; i < 7; i++)
	{
		pair<int, char> value;
		value.first = i;
		value.second = '0' + i;

		auto check_if_equal_pair = [&value](const pair<const int, char> pair_to_check)
		{ return pair_to_check.first == value.first && pair_to_check.second == value.second; };

		REQUIRE(bud::find_if(my_map.begin(), my_map.end(), check_if_equal_pair) != my_map.end());
	}

	unordered_map<int, char> my_other_set;
	REQUIRE(my_other_set.begin() == my_other_set.end());
}
