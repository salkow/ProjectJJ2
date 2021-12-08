#include "../lib/include/catch2/catch.hpp"
#include "../src/array.h"

using bud::array;

TEST_CASE("Empty array", "[empty_array")
{
	array<int, 0> arr;

	REQUIRE(arr.empty());
	REQUIRE(arr.size() == 0);

	REQUIRE(arr.begin() == arr.end());

	/* REQUIRE(arr.front() == arr.back()); */

	REQUIRE(arr.data() == nullptr);
}

TEST_CASE("Bracket operator array", "[bracket_operator_array]")
{
	array<int, 2> arr;

	REQUIRE(arr.size() == 2);
	REQUIRE(!arr.empty());

	arr[0] = 8;
	arr[1] = 6;

	REQUIRE(arr[0] == 8);
	REQUIRE(arr[1] == 6);

	REQUIRE(arr.size() == 2);
	REQUIRE(!arr.empty());
}

TEST_CASE("Initializer list array", "[initializer_list_array]")
{
	// C
	array<int, 3> arr = {27, 36, 48};

	REQUIRE(arr[0] == 27);
	REQUIRE(arr[1] == 36);
	REQUIRE(arr[2] == 48);

	REQUIRE(arr.size() == 3);
	REQUIRE(!arr.empty());
}

TEST_CASE("Data access array")
{
	array<int, 3> arr = {27, 36, 48};

	REQUIRE(arr.front() == 27);
	REQUIRE(arr.back() == 48);

	REQUIRE(arr.data()[1] == 36);
}

TEST_CASE("Fill array")
{
	array<int, 3> arr;
	arr.fill(9);

	REQUIRE(arr[0] == 9);
	REQUIRE(arr[1] == 9);
	REQUIRE(arr[2] == 9);
}

TEST_CASE("Iterators array", "[iterators_array]")
{
	array<int, 3> arr;
	arr.fill(9);

	int sum = 0;

	for (const auto& i : arr)
	{
		sum += i;
	}

	REQUIRE(sum == 27);

	auto my_it = arr.begin();
	my_it++;
	*my_it = 8;

	--my_it;
	*my_it = 7;

	my_it += 2;
	*my_it = 6;

	REQUIRE(arr[0] == 7);
	REQUIRE(arr[1] == 8);
	REQUIRE(arr[2] == 6);

	REQUIRE(arr.begin() < arr.end());
	REQUIRE(arr.begin() + 3 == arr.end());
	REQUIRE(arr.begin() + 2 == arr.end() - 1);
}
