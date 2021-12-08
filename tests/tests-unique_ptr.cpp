#include "../lib/include/catch2/catch.hpp"
#include "../src/unique_ptr.h"

#include <utility>

using bud::make_unique;
using bud::unique_ptr;

TEST_CASE("Construct empty unique_ptr", "[construct_empty_unique_ptr]")
{
	unique_ptr<int> empty_1;
	REQUIRE(empty_1.get() == nullptr);

	unique_ptr<int> empty_2(nullptr);
	REQUIRE(empty_2.get() == nullptr);
}

TEST_CASE("Construct unique_ptr", "[construct_unique_ptr]")
{
	unique_ptr<int> x(new int(3));
	REQUIRE(*x == 3);
}

TEST_CASE("Move operations", "[move_operations")
{
	unique_ptr<int> x_1(new int(4));
	REQUIRE(*x_1 == 4);

	unique_ptr<int> x_2(std::move(x_1));
	REQUIRE(*x_2 == 4);

	unique_ptr<int> x_3;
	x_3 = std::move(x_2);
	REQUIRE(*x_3 == 4);
}

TEST_CASE("Reset unique_ptr", "[reset_unique_ptr]")
{
	unique_ptr<int> x(new int(5));
	REQUIRE(*x == 5);

	x.reset(new int(3));
	REQUIRE(*x == 3);

	x.reset();
	REQUIRE(x.get() == nullptr);

	unique_ptr<int> x_1(new int(7));
	REQUIRE(*x_1 == 7);

	x_1 = nullptr;
	REQUIRE(x_1.get() == nullptr);
}

TEST_CASE("Release", "[release]")
{
	unique_ptr<int> x(new int(9));
	REQUIRE(*x == 9);

	int* x_ptr = x.release();
	REQUIRE(*x_ptr == 9);

	delete x_ptr;
}

TEST_CASE("Dereference operator", "[dereference_operator")
{
	unique_ptr<int> x(new int(15));

	REQUIRE(*x == 15);
}

TEST_CASE("Bool operator", "[bool_operator")
{
	unique_ptr<int> x;

	REQUIRE(!x);

	x.reset(new int(9));

	REQUIRE(x);
}

class Base
{
public:
	explicit Base(int y) : m_y(y) {}

	int m_y;
};

class Derived : public Base
{
public:
	explicit Derived(int y) : Base(y) {}
};

TEST_CASE("Derived class pointer to a base class pointer", "[derived_pointer_to_base_pointer]")
{
	unique_ptr<Base> x(new Derived(9));

	REQUIRE(x->m_y == 9);

	unique_ptr<Derived> derived(new Derived(8));

	unique_ptr<Base> base = std::move(derived);

	REQUIRE(base->m_y == 8);

	unique_ptr<Base> other_base(new Base(9));
	other_base = std::move(base);

	REQUIRE(other_base->m_y == 8);

	unique_ptr<Derived> third_base(new Derived(8));

	unique_ptr<Base> forth_base(new Base(18));
	forth_base = std::move(third_base);
	REQUIRE(forth_base->m_y == 8);

	unique_ptr<Derived> fifth_base(new Derived(28));
	unique_ptr<Base> sixth(std::move(fifth_base));
	REQUIRE(sixth->m_y == 28);
}

TEST_CASE("make_unique", "[make_unique]")
{
	auto x(make_unique<int>(0));

	REQUIRE(*x == 0);
}

TEST_CASE("null unique_ptr", "[null_unique_ptr]")
{
    unique_ptr<int> x(new int(3));  

    unique_ptr<int> y = nullptr;

    x = std::move(y);

	REQUIRE(x == nullptr);
}
