#include "../lib/include/catch2/catch.hpp"

#include "../src/unordered_set.h"
#include "../src/my_string.h"

using bud::pair;
using bud::string;
using bud::unordered_set;

TEST_CASE("Search for items in empty set.", "[search_in_empty_set]")
{
	unordered_set<int> my_set(2);

	REQUIRE(my_set[2345] == nullptr);
	REQUIRE(my_set[93] == nullptr);
	REQUIRE(my_set[0] == nullptr);
	REQUIRE(my_set[231] == nullptr);
	REQUIRE(my_set[2] == nullptr);
	REQUIRE(my_set[3] == nullptr);

	REQUIRE(my_set.empty());
}

TEST_CASE("Insert items to unordered_set.", "[insert_to_unordered_set]")
{
	unordered_set<int> my_set(3);

	auto my_pair = my_set.insert(1);

	REQUIRE(my_pair.second == true);

	my_set.insert(2);
	my_set.insert(3);
	my_set.insert(4);
	my_set.insert(5);
	my_set.insert(6);
	my_set.insert(7);

	REQUIRE(*my_set[1] == 1);
	REQUIRE(*my_set[2] == 2);
	REQUIRE(*my_set[3] == 3);
	REQUIRE(*my_set[4] == 4);
	REQUIRE(*my_set[5] == 5);
	REQUIRE(*my_set[6] == 6);
	REQUIRE(*my_set[7] == 7);

	REQUIRE(my_set.size() == 7);
}

TEST_CASE("Store many items in one unordered set list.", "[multiple_items_one_unordered_set_list]")
{
	unordered_set<int> my_set(1);

	my_set.insert(54);
	my_set.insert(99);
	my_set.insert(18);
	my_set.insert(60);

	REQUIRE(*my_set[54] == 54);
	REQUIRE(*my_set[99] == 99);
	REQUIRE(*my_set[18] == 18);
	REQUIRE(*my_set[60] == 60);

	REQUIRE(my_set.size() == 4);
}

TEST_CASE("Try to find non existing item in unordered_set.",
		  "[find_non_existing_item_unordered_set]")
{
	unordered_set<int> my_set(100);

	my_set.insert(54);
	my_set.insert(99);
	my_set.insert(18);
	my_set.insert(60);

	REQUIRE(my_set[53] == nullptr);
	REQUIRE(my_set[23] == nullptr);
	REQUIRE(my_set[0] == nullptr);
	REQUIRE(my_set[999] == nullptr);
	REQUIRE(my_set[111] == nullptr);
}

TEST_CASE("Insert existing key in unordered_set.", "[insert_existing_key_unordered_set]")
{
	unordered_set<int> my_set;

	auto return_value = my_set.insert(54);
	REQUIRE(return_value.second == true);

	return_value = my_set.insert(54);
	REQUIRE(return_value.second == false);

	REQUIRE(*my_set[54] == 54);
}

TEST_CASE("Erase element unordered_set", "[erase_element_unordered_set]")
{
	unordered_set<int> my_set(7);

	REQUIRE(my_set.erase(54) == 0);
	REQUIRE(my_set.erase(99) == 0);
	REQUIRE(my_set.erase(18) == 0);
	REQUIRE(my_set.erase(60) == 0);

	my_set.insert(54);
	my_set.insert(99);
	my_set.insert(18);
	my_set.insert(60);

	REQUIRE(my_set.erase(54) == 1);
	REQUIRE(my_set.size() == 3);

	REQUIRE(*my_set[99] == 99);
	REQUIRE(*my_set[18] == 18);
	REQUIRE(*my_set[60] == 60);

	REQUIRE(my_set.erase(62) == 0);
	REQUIRE(my_set.erase(61) == 0);
	REQUIRE(my_set.size() == 3);

	REQUIRE(my_set.erase(18) == 1);
	REQUIRE(my_set.size() == 2);

	REQUIRE(*my_set[99] == 99);
	REQUIRE(*my_set[60] == 60);

	REQUIRE(my_set.erase(60) == 1);
	REQUIRE(my_set.size() == 1);

	REQUIRE(my_set.erase(99) == 1);
	REQUIRE(my_set.size() == 0);

	REQUIRE(my_set[54] == nullptr);
	REQUIRE(my_set[99] == nullptr);
	REQUIRE(my_set[18] == nullptr);
	REQUIRE(my_set[60] == nullptr);

	REQUIRE(my_set.erase(54) == 0);
	REQUIRE(my_set.erase(99) == 0);
	REQUIRE(my_set.erase(18) == 0);
	REQUIRE(my_set.erase(60) == 0);
}

TEST_CASE("Rehash unordered_set", "[rehash_unordered_set]")
{
	unordered_set<int> my_set(2);

	my_set.insert(2);
	my_set.insert(3);
	my_set.insert(4);
	my_set.insert(5);
	my_set.insert(6);
	my_set.insert(7);

	my_set.rehash(1);
	my_set.rehash(2);
	my_set.rehash(3);
	my_set.rehash(4);
	my_set.rehash(100);

	REQUIRE(*my_set[2] == 2);
	REQUIRE(*my_set[3] == 3);
	REQUIRE(*my_set[4] == 4);
	REQUIRE(*my_set[5] == 5);
	REQUIRE(*my_set[6] == 6);
	REQUIRE(*my_set[7] == 7);
}

TEST_CASE("unordered_set class value", "[unordered_set_class_value]")
{
	unordered_set<string> my_set(2);

	my_set.insert("2");
	my_set.insert("3");
	my_set.insert("4");
	my_set.insert("5");
	my_set.insert("6");
	my_set.insert("7");

	my_set.rehash(1);
	my_set.rehash(2);
	my_set.rehash(3);
	my_set.rehash(4);
	my_set.rehash(100);

	REQUIRE(*my_set["2"] == "2");
	REQUIRE(*my_set["3"] == "3");
	REQUIRE(*my_set["4"] == "4");
	REQUIRE(*my_set["5"] == "5");
	REQUIRE(*my_set["6"] == "6");
	REQUIRE(*my_set["7"] == "7");
}

TEST_CASE("unordered_set class key", "[unordered_set_class_key]")
{
	unordered_set<string> my_set(2);

	my_set.insert("b");
	my_set.insert("c");
	my_set.insert("d");
	my_set.insert("e");
	my_set.insert("f");
	my_set.insert("g");

	my_set.rehash(1);
	my_set.rehash(2);
	my_set.rehash(3);
	my_set.rehash(4);
	my_set.rehash(100);

	REQUIRE(*my_set["b"] == "b");
	REQUIRE(*my_set["c"] == "c");
	REQUIRE(*my_set["d"] == "d");
	REQUIRE(*my_set["e"] == "e");
	REQUIRE(*my_set["f"] == "f");
	REQUIRE(*my_set["g"] == "g");
}

TEST_CASE("unordered_set iterators", "[unordered_set_iterators]")
{
	unordered_set<int> my_set(7);

	for (const auto& bucket : my_set.data())
		REQUIRE(bucket.begin() == bucket.end());

	my_set.insert(1);
	my_set.insert(2);
	my_set.insert(3);
	my_set.insert(4);
	my_set.insert(5);
	my_set.insert(6);

	REQUIRE(my_set.size() == 6);

	REQUIRE(bud::find(my_set.begin(), my_set.end(), 1) != my_set.end());
	REQUIRE(bud::find(my_set.begin(), my_set.end(), 2) != my_set.end());
	REQUIRE(bud::find(my_set.begin(), my_set.end(), 3) != my_set.end());
	REQUIRE(bud::find(my_set.begin(), my_set.end(), 4) != my_set.end());
	REQUIRE(bud::find(my_set.begin(), my_set.end(), 5) != my_set.end());
	REQUIRE(bud::find(my_set.begin(), my_set.end(), 6) != my_set.end());

	my_set.insert(7);
	REQUIRE(my_set.size() == 7);

	REQUIRE(bud::find(my_set.begin(), my_set.end(), 1) != my_set.end());
	REQUIRE(bud::find(my_set.begin(), my_set.end(), 2) != my_set.end());
	REQUIRE(bud::find(my_set.begin(), my_set.end(), 3) != my_set.end());
	REQUIRE(bud::find(my_set.begin(), my_set.end(), 4) != my_set.end());
	REQUIRE(bud::find(my_set.begin(), my_set.end(), 5) != my_set.end());
	REQUIRE(bud::find(my_set.begin(), my_set.end(), 6) != my_set.end());
	REQUIRE(bud::find(my_set.begin(), my_set.end(), 7) != my_set.end());

	REQUIRE(bud::find(my_set.begin(), my_set.end(), 8) == my_set.end());
	REQUIRE(bud::find(my_set.begin(), my_set.end(), 9) == my_set.end());
	REQUIRE(bud::find(my_set.begin(), my_set.end(), 10) == my_set.end());
	REQUIRE(bud::find(my_set.begin(), my_set.end(), 11) == my_set.end());

	unordered_set<int> my_other_set;
	REQUIRE(my_other_set.begin() == my_other_set.end());
}

TEST_CASE("Unordered_set merge", "[unordered_set_merge]")
{
	unordered_set<int> first_set;

	first_set.insert(1);
	first_set.insert(2);
	first_set.insert(3);
	first_set.insert(4);
	first_set.insert(5);
	first_set.insert(6);

	unordered_set<int> second_set;

	second_set.insert(7);
	second_set.insert(8);
	second_set.insert(9);

	first_set.merge(std::move(second_set));

	REQUIRE(bud::find(first_set.begin(), first_set.end(), 1) != first_set.end());
	REQUIRE(bud::find(first_set.begin(), first_set.end(), 2) != first_set.end());
	REQUIRE(bud::find(first_set.begin(), first_set.end(), 3) != first_set.end());
	REQUIRE(bud::find(first_set.begin(), first_set.end(), 4) != first_set.end());
	REQUIRE(bud::find(first_set.begin(), first_set.end(), 5) != first_set.end());
	REQUIRE(bud::find(first_set.begin(), first_set.end(), 6) != first_set.end());
	REQUIRE(bud::find(first_set.begin(), first_set.end(), 7) != first_set.end());
	REQUIRE(bud::find(first_set.begin(), first_set.end(), 8) != first_set.end());
	REQUIRE(bud::find(first_set.begin(), first_set.end(), 9) != first_set.end());
}

TEST_CASE("Clear unordered_set", "clear_unordered_set")
{
	unordered_set<int> my_set;

	my_set.insert(1);
	my_set.insert(2);
	my_set.insert(3);
	my_set.insert(4);
	my_set.insert(5);
	my_set.insert(6);

	my_set.clear();

	REQUIRE(my_set.empty());

	REQUIRE(bud::find(my_set.begin(), my_set.end(), 1) == my_set.end());
	REQUIRE(bud::find(my_set.begin(), my_set.end(), 2) == my_set.end());
	REQUIRE(bud::find(my_set.begin(), my_set.end(), 3) == my_set.end());
	REQUIRE(bud::find(my_set.begin(), my_set.end(), 4) == my_set.end());
	REQUIRE(bud::find(my_set.begin(), my_set.end(), 5) == my_set.end());
	REQUIRE(bud::find(my_set.begin(), my_set.end(), 6) == my_set.end());
	REQUIRE(bud::find(my_set.begin(), my_set.end(), 7) == my_set.end());
	REQUIRE(bud::find(my_set.begin(), my_set.end(), 8) == my_set.end());
	REQUIRE(bud::find(my_set.begin(), my_set.end(), 9) == my_set.end());
}
