
#include "../lib/include/catch2/catch.hpp"
#include "../src/vector.h"
#include "../src/my_string.h"

using bud::string;
using bud::vector;

TEST_CASE("Check if a new vector is empty.", "[empty_vector]")
{
	vector<int> empty_vector;

	REQUIRE(empty_vector.empty());
}

TEST_CASE("Check capacity after reserve in vector.", "[capacity_after_reserve_vector]")
{
	vector<int> empty_vector;
	empty_vector.reserve(10);

	REQUIRE(empty_vector.capacity() == 10);
}

TEST_CASE("Check if default constructed elements exist in vector.",
		  "[check_default_constructed_elements]")
{
	vector<int> my_vector(3);

	REQUIRE(my_vector[0] == 0);
	REQUIRE(my_vector[1] == 0);
	REQUIRE(my_vector[2] == 0);

	REQUIRE(my_vector.at(0) == 0);
	REQUIRE(my_vector.at(1) == 0);
	REQUIRE(my_vector.at(2) == 0);
}

TEST_CASE("Check if elements added from constructor exist in vector",
		  "[check_elements_from_constructor_exist_vector]")
{
	vector<int> my_vector(2, 9);

	REQUIRE(my_vector[0] == 9);
	REQUIRE(my_vector[1] == 9);

	REQUIRE(my_vector.at(0) == 9);
	REQUIRE(my_vector.at(1) == 9);
}

TEST_CASE("Check size after adding some elements in vector.",
		  "[check_size_after_adding_elements_vector]")
{
	vector<int> vec_1(2, 9);
	REQUIRE(vec_1.size() == 2);

	vector<int> vec_2(3);
	REQUIRE(vec_2.size() == 3);
}

TEST_CASE("Emplace back some elements in vector.", "[emplace_back_elements_vector]")
{
	vector<int> my_vec;
	my_vec.reserve(2);

	my_vec.emplace_back(13);
	my_vec.emplace_back(23);
	my_vec.emplace_back(33);

	REQUIRE(my_vec.size() == 3);

	REQUIRE(my_vec[0] == 13);
	REQUIRE(my_vec[1] == 23);
	REQUIRE(my_vec[2] == 33);
}

TEST_CASE("Push back some elements in vector.", "[push_back_elements_vector]")
{
	vector<int> my_vec;
	my_vec.reserve(2);

	my_vec.push_back(13);
	my_vec.push_back(23);
	my_vec.push_back(33);

	REQUIRE(my_vec.size() == 3);

	REQUIRE(my_vec[0] == 13);
	REQUIRE(my_vec[1] == 23);
	REQUIRE(my_vec[2] == 33);
}

TEST_CASE("Clear vector.", "[clear_vector]")
{
	vector<int> my_vec;

	my_vec.push_back(13);
	my_vec.emplace_back(33);

	my_vec.clear();

	REQUIRE(my_vec.size() == 0);
}

TEST_CASE("Pop back some elements in vector.", "[pop_back_elements_vector]")
{
	vector<int> my_vec;
	my_vec.reserve(2);

	my_vec.push_back(13);
	my_vec.push_back(23);
	my_vec.emplace_back(33);
	my_vec.emplace_back(43);

	REQUIRE(my_vec.size() == 4);

	my_vec.pop_back();
	REQUIRE(my_vec.size() == 3);

	my_vec.pop_back();
	REQUIRE(my_vec.size() == 2);
}

TEST_CASE("Test iterators in vector.", "[test_iterators_vector]")
{
	int sum = 0;

	vector<int> my_vec;
	my_vec.reserve(5);

	my_vec.push_back(1);
	my_vec.emplace_back(2);
	my_vec.push_back(3);
	my_vec.emplace_back(4);
	my_vec.push_back(5);
	my_vec.emplace_back(6);

	for (const auto& x : my_vec)
		sum += x;

	REQUIRE(sum == 21);
}

TEST_CASE("Add many items vector", "[many_items_to_vector]")
{
	vector<int> vec;

	for (int i = 0; i < 10000; i++)
		vec.emplace_back(i);

	REQUIRE(vec.size() == 10000);

	REQUIRE(vec[0] == 0);
	REQUIRE(vec[9999] == 9999);
	REQUIRE(vec[5821] == 5821);
}

TEST_CASE("Copy construct empty vector", "[copy_construct_empty_vector]")
{
	vector<int> vec1;
	vector<int> vec2 = vec1;

	REQUIRE(vec2.size() == 0);
}

TEST_CASE("Copy vector")
{
	vector<int> vec_1;
	vec_1.emplace_back(3);
	vec_1.emplace_back(2);

	vector<int> vec_2 = vec_1;

	REQUIRE(vec_1 == vec_2);

	vector<int> vec_3;
	vec_3 = vec_1;

	REQUIRE(vec_1 == vec_3);
}

TEST_CASE("Erase element vector", "[erase_element_vector]")
{
	vector<bud::string> vec_1;
	vec_1.emplace_back("1");
	vec_1.emplace_back("2");
	vec_1.emplace_back("3");

	auto result_it = vec_1.erase(vec_1.begin() + 1);
	REQUIRE(result_it == vec_1.begin() + 1);

	REQUIRE(vec_1[0] == "1");
	REQUIRE(vec_1[1] == "3");

	REQUIRE(vec_1.size() == 2);

	auto vec_2 = vec_1;

	result_it = vec_1.erase(vec_1.begin());
	REQUIRE(result_it == vec_1.begin());

	REQUIRE(vec_1[0] == "3");

	REQUIRE(vec_1.size() == 1);

	result_it = vec_2.erase(vec_2.begin() + 1);
	REQUIRE(result_it == vec_2.begin() + 1);

	REQUIRE(vec_2[0] == "1");

	REQUIRE(vec_2.size() == 1);

	result_it = vec_1.erase(vec_1.begin());
	REQUIRE(result_it == vec_1.begin());
	REQUIRE(result_it == vec_1.end());

	REQUIRE(vec_1.empty());
}
