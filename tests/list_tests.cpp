#include "../lib/include/catch2/catch.hpp"

#include "../src/array.h"
#include "../src/list.h"
#include "../src/my_string.h"

using bud::list;
using bud::string;

#include <iostream>

template <class T, class... Params>
void check_list_iterators(const list<T>& t_list, Params... t_params)
{
	bud::array<T, sizeof...(t_params)> params = {t_params...};

	auto list_it = t_list.begin();
	auto param_it = params.begin();

	REQUIRE(t_list.size() == params.size());

	for (; list_it != t_list.end(); ++list_it, ++param_it)
		REQUIRE(*list_it == *param_it);

	list_it = t_list.end();
	param_it = params.end();

	for (; list_it != t_list.end(); --list_it, --param_it)
		REQUIRE(*list_it == *param_it);
}

TEST_CASE("Check if a new list is empty.", "[empty_list]")
{
	list<string> empty_list;
	REQUIRE(empty_list.empty());
}

TEST_CASE("emplace front empty list.", "[emplace_front_empty_list]")
{
	list<string> my_list;

	my_list.emplace_front("9");

	REQUIRE(my_list.front() == "9");
	REQUIRE(my_list.back() == "9");

	REQUIRE(my_list.size() == 1);

	check_list_iterators(my_list, "9");
}

TEST_CASE("emplace front list.", "[emplace_front_list]")
{
	list<string> my_list;

	my_list.emplace_front("7");
	my_list.emplace_front("8");
	my_list.emplace_front("9");

	check_list_iterators(my_list, "9", "8", "7");
}

TEST_CASE("Pop front one item list", "[pop_front_one_item_list]")
{
	list<string> my_list;

	my_list.emplace_front("7");

	my_list.pop_front();

	REQUIRE(my_list.empty());

	REQUIRE(my_list.begin() == my_list.end());
}

TEST_CASE("Pop front list", "[pop_front_list]")
{
	list<string> my_list;

	my_list.emplace_front("7");
	my_list.emplace_front("8");
	my_list.emplace_front("9");

	my_list.pop_front();

	check_list_iterators(my_list, "8", "7");

	my_list.pop_front();

	check_list_iterators(my_list, "7");

	my_list.pop_front();

	REQUIRE(my_list.empty());

	REQUIRE(my_list.begin() == my_list.end());
}

TEST_CASE("emplace back empty list.", "[emplace_back_empty_list]")
{
	list<string> my_list;

	my_list.emplace_back("9");

	REQUIRE(my_list.front() == "9");
	REQUIRE(my_list.back() == "9");

	REQUIRE(my_list.size() == 1);

	check_list_iterators(my_list, "9");
}

TEST_CASE("emplace back list.", "[emplace_back_list]")
{
	list<string> my_list;

	my_list.emplace_back("7");
	my_list.emplace_back("8");
	my_list.emplace_back("9");

	check_list_iterators(my_list, "7", "8", "9");
}

TEST_CASE("Pop back one item list", "[pop_back_one_item_list]")
{
	list<string> my_list;

	my_list.emplace_front("7");

	my_list.pop_back();

	REQUIRE(my_list.empty());

	REQUIRE(my_list.begin() == my_list.end());
}

TEST_CASE("Pop back list", "[pop_back_list]")
{
	list<string> my_list;

	my_list.emplace_front("7");
	my_list.emplace_front("8");
	my_list.emplace_front("9");

	my_list.pop_back();

	check_list_iterators(my_list, "9", "8");

	my_list.pop_back();

	check_list_iterators(my_list, "9");

	my_list.pop_back();

	REQUIRE(my_list.empty());

	REQUIRE(my_list.begin() == my_list.end());
}

TEST_CASE("Emplace empty list", "emplace_empty_list")
{
	list<string> list_1;

	list_1.emplace(list_1.begin(), "4");

	check_list_iterators(list_1, "4");

	list<string> list_2;

	list_2.emplace(list_2.end(), "4");

	check_list_iterators(list_2, "4");
}

TEST_CASE("Emplace list begin", "emplace_list_begin")
{
	list<string> my_list;

	my_list.emplace(my_list.begin(), "4");
	my_list.emplace(my_list.begin(), "5");

	check_list_iterators(my_list, "5", "4");
}

TEST_CASE("Emplace list end", "emplace_list_end")
{
	list<string> my_list;

	my_list.emplace(my_list.end(), "4");
	my_list.emplace(my_list.end(), "5");

	check_list_iterators(my_list, "4", "5");
}

TEST_CASE("Emplace list middle", "emplace_list_middle")
{
	list<string> my_list;

	my_list.emplace(my_list.begin(), "4");
	my_list.emplace(my_list.end(), "5");

	auto it = my_list.begin();
	++it;

	my_list.emplace(it, "4.5");

	check_list_iterators(my_list, "4", "4.5", "5");
}

TEST_CASE("Erase list one item", "erase_list_one_item")
{
	list<string> my_list;

	my_list.emplace(my_list.begin(), "4");

	my_list.erase(my_list.begin());

	REQUIRE(my_list.empty());

	REQUIRE(my_list.begin() == my_list.end());
}

TEST_CASE("Erase list first item", "erase_list_first_item")
{
	list<string> my_list;

	my_list.emplace(my_list.begin(), "6");
	my_list.emplace(my_list.begin(), "5");
	my_list.emplace(my_list.begin(), "4");

	REQUIRE(*(my_list.erase(my_list.begin())) == "5");

	check_list_iterators(my_list, "5", "6");
}

TEST_CASE("Erase list middle item", "erase_list_middle_item")
{
	list<string> my_list;

	my_list.emplace(my_list.begin(), "6");
	my_list.emplace(my_list.begin(), "5");
	my_list.emplace(my_list.begin(), "4");

	auto it = my_list.begin();
	++it;

	it = my_list.erase(it);

	REQUIRE(*it == "6");

	check_list_iterators(my_list, "4", "6");

	it = my_list.erase(it);

	REQUIRE(it == my_list.end());

	check_list_iterators(my_list, "4");

	it = my_list.erase(my_list.begin());

	REQUIRE(it == my_list.end());

	REQUIRE(my_list.empty());

	REQUIRE(my_list.begin() == my_list.end());
}

TEST_CASE("Erase list last item", "erase_list_last_item")
{
	list<string> my_list;

	my_list.emplace(my_list.begin(), "6");
	my_list.emplace(my_list.begin(), "5");
	my_list.emplace(my_list.begin(), "4");

	auto it = my_list.end();
	--it;

	REQUIRE(*it == "6");

	it = my_list.erase(it);

	REQUIRE(it == my_list.end());

	check_list_iterators(my_list, "4", "5");

	it = my_list.end();
	--it;

	// REQUIRE(*it == "5");

	// it = my_list.erase(it);

	// REQUIRE(it == my_list.end());

	// check_list_iterators(my_list, "4");

	// it = my_list.erase(my_list.begin());

	// REQUIRE(it == my_list.end());

	// REQUIRE(my_list.empty());

	// REQUIRE(my_list.begin() == my_list.end());
}