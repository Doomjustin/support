#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_template_test_macros.hpp>

#include "support/collections/List.h"


TEMPLATE_TEST_CASE("List constructor", "[List]", int, float)
{
    SECTION("List()")
    {
        support::List<TestType> list{};

        REQUIRE(list.size() == 0);
        REQUIRE(list.capacity() == 4);
    }

    SECTION("List(init_capacity, init_value)")
    {
        support::List list( 10, 12 );

        REQUIRE(list.size() == 10);
        REQUIRE(list.capacity() == 10);

        for (auto& element: list)
            REQUIRE(element == 12);
    }

    SECTION("List(std::initialized_list<>)")
    {
        support::List list{ 1, 2, 3, 4, 5 };

        REQUIRE(list.size() == 5);
        REQUIRE(list.capacity() == 5);

        for (int i = 0; i < 4; ++i)
            REQUIRE(list[i] == i + 1);
    }
}

TEMPLATE_TEST_CASE("iterator", "[List]", int, float)
{
    support::List<TestType> list{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    const support::List<TestType> const_list{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

    REQUIRE(list.size() == 10);
    REQUIRE(list.capacity() == 10);

    SECTION("begin(), end()", "[nonconst]")
    {
        int i = 0;
        for (auto beg = list.begin(); beg != list.end(); ++beg)
            REQUIRE(*beg == i++);
    }

    SECTION("cbegin(), cend()", "[nonconst]")
    {
        int i = 0;
        for (auto beg = list.cbegin(); beg != list.cend(); ++beg)
            REQUIRE(*beg == i++);
    }

    SECTION("begin(), end()", "[const]")
    {
        int i = 0;
        for (auto beg = const_list.begin(); beg != const_list.end(); ++beg)
            REQUIRE(*beg == i++);
    }

    SECTION("cbegin(), cend()", "[const]")
    {
        int i = 0;
        for (auto beg = const_list.cbegin(); beg != const_list.cend(); ++beg)
            REQUIRE(*beg == i++);
    }
}

TEST_CASE("modify operator", "[List]")
{
    support::List<int> list{};

    REQUIRE(list.size() == 0);
    REQUIRE(list.capacity() == 4);

    SECTION("push_back single value")
    {
        list.push_back(1);

        REQUIRE(list.size() == 1);
        REQUIRE(list.capacity() == 4);
        REQUIRE(list.back() == 1);
    }

    SECTION("push_back multiple value")
    {
        for (int i = 0; i < 100; ++i)
            list.push_back(i);

        REQUIRE(list.size() == 100);

        for (int i = 0; i < 100; ++i)
            REQUIRE(list[i] == i);
    }

    SECTION("insert")
    {
        for (int i = 0; i < 100; ++i)
            list.push_back(i);

        list.insert(10, 1111);

        REQUIRE(list.size() == 101);
        REQUIRE(list[10] == 1111);

        for (int i = 0; i < 10; ++i)
            REQUIRE(list[i] == i);

        for (int i = 11; i < 101; ++i)
            REQUIRE(list[i] == i - 1);
    }
}