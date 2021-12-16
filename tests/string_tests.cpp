#include "../lib/include/catch2/catch.hpp"
#include "../src/my_string.h"

using bud::string;

TEST_CASE("Fill a string", "[fill_string]")
{
	string x(2, 'a');
	REQUIRE(x[0] == 'a');
	REQUIRE(x[1] == 'a');

	REQUIRE(x.size() == 2);
	REQUIRE(!x.empty());

	REQUIRE(x == "aa");
}

TEST_CASE("Copy string", "[copy_string]")
{
	string x1("123");
	REQUIRE(x1[0] == '1');
	REQUIRE(x1[1] == '2');
	REQUIRE(x1[2] == '3');
	REQUIRE(x1[3] == '\0');

	string x2 = x1;
	REQUIRE(x1 == x2);
	REQUIRE(!x2.empty());
	REQUIRE(x1 == x2);

	string x3;
	x3 = x2;
	REQUIRE(x2 == x3);
	REQUIRE(!x3.empty());
	REQUIRE(x3 == x2);

	string x4("abc");
	x4.reserve(15);
	REQUIRE(x3 != x4);
	x4 = x3;
	REQUIRE(x3 == x4);
	REQUIRE(!x4.empty());
	REQUIRE(x3 == x4);

	string x5;
	x5.reserve(4);
	x5 = "cba";
	REQUIRE(x5 == "cba");
	REQUIRE(x5 != "bca");
}

TEST_CASE("Self assignment string.", "[self_assignment_string]")
{
	string word_1("this is a string.");
	word_1 = word_1;
	REQUIRE(word_1 == word_1);
}

TEST_CASE("Move string", "[move_string]")
{
	string x1("123");

	string x2 = std::move(x1);
	REQUIRE(x2 == "123");

	string x3;
	x3 = std::move(x2);
	REQUIRE(x3 == "123");
}

TEST_CASE("Reserve and capacity", "[reserve_and_capacity]")
{
	string x = "nza";
	x.reserve(19);

	REQUIRE(x.capacity() == 19);
	REQUIRE(x == "nza");
}

TEST_CASE("c_str", "[c_str]")
{
	string x = "pop";
	REQUIRE(!strcmp(x.c_str(), "pop"));
}

TEST_CASE("push_pop_back", "[push_pop_back]")
{
	string x;
	x.push_back('a');

	REQUIRE(x == "a");

	x.push_back('b');
	REQUIRE(x == "ab");

	x.reserve(3);

	x.push_back('c');

	REQUIRE(x == "abc");

	x.pop_back();

	REQUIRE(x == "ab");
}

TEST_CASE("clear string", "[clear_string]")
{
	string x = "car";
	x.clear();
	REQUIRE(x == "");
	REQUIRE(x.empty());
	REQUIRE(x.size() == 0);
}

TEST_CASE("Iterators string", "[iterators_string]")
{
	string x = "car";

	auto it = x.begin();
	REQUIRE(*it == 'c');

	it++;
	REQUIRE(*it == 'a');

	it++;
	REQUIRE(*it == 'r');

	it++;
	REQUIRE(it == x.end());
}

TEST_CASE("string from char* and size", "string_from_char_pointer_and_size")
{
	char a[] = "1234";

	string a_str(a, strlen(a));

	REQUIRE(a_str == a);
}
