
#define TCB_SPAN_NO_DEPRECATION_WARNINGS
#define TCB_SPAN_THROW_ON_CONTRACT_VIOLATION
#include <tcb/span.hpp>

#include "catch.hpp"

#include <vector>

using tcb::make_span;
using tcb::span;

#define TEST(expr) REQUIRE_THROWS_AS(expr, std::logic_error)

TEST_CASE("span(ptr, length)")
{
    std::vector<int> vec{1, 2, 3};

    TEST((tcb::span<int, 3>{nullptr, 1}));
    TEST((tcb::span<int, 3>{vec.data(), 1}));
}

TEST_CASE("span(ptr, ptr)")
{
    std::vector<int> vec{1, 2, 3};

    TEST((tcb::span<int, 3>{nullptr, nullptr}));
    TEST((tcb::span<int, 3>{vec.data(), vec.data() + 1}));
}

TEST_CASE("span(container)")
{
    SECTION("non-const")
    {
        std::vector<int> vec{1, 2, 3};
        TEST((tcb::span<int, 2>{vec}));
    }

    SECTION("const")
    {
        const std::vector<int> vec{1, 2, 3};
        TEST((tcb::span<const int, 2>{vec}));
    }
}

TEST_CASE("fixed-size subspans")
{
    SECTION("first<N>()")
    {
        std::vector<int> vec{1, 2, 3};
        auto s = make_span(vec);
        TEST(s.first<-1>());
        TEST(s.first<4>());
    }

    SECTION("last<N>()")
    {
        std::vector<int> vec{1, 2, 3};
        auto s = make_span(vec);
        TEST(s.last<-1>());
        TEST(s.last<4>());
    }

    SECTION("subspan<N>()")
    {
        std::vector<int> vec{1, 2, 3, 4, 5};
        auto s = make_span(vec);
        TEST(s.subspan<-1>());
        TEST(s.subspan<6>());
    }

    SECTION("subspan<N, M>()")
    {
        std::vector<int> vec{1, 2, 3, 4, 5};
        auto s = make_span(vec);
        // TEST((s.subspan<0, -2>())); // turns out this gets static_assert'd in
        // the return type
        TEST((s.subspan<0, 6>()));
    }
}

TEST_CASE("dynamic-size subspans")
{
    SECTION("first(n)")
    {
        std::vector<int> vec{1, 2, 3};
        auto s = make_span(vec);
        TEST(s.first(-1));
        TEST(s.first(4));
    }

    SECTION("last(n)")
    {
        std::vector<int> vec{1, 2, 3};
        auto s = make_span(vec);
        TEST(s.last(-1));
        TEST(s.last(4));
    }

    SECTION("subspan(n)")
    {
        std::vector<int> vec{1, 2, 3, 4, 5};
        auto s = make_span(vec);
        TEST(s.subspan(-1));
        TEST(s.subspan(6));
    }

    SECTION("subspan(n, m)")
    {
        std::vector<int> vec{1, 2, 3, 4, 5};
        auto s = make_span(vec);
        TEST(s.subspan(0, -2));
        TEST(s.subspan(0, 6));
    }
}

TEST_CASE("Member access")
{
    std::vector<int> vec{1, 2, 3};
    auto s = make_span(vec);

    TEST(s[-2]);
    TEST(s[42]);

    TEST(s(-2));
    TEST(s(42));
}

TEST_CASE("front() and back()")
{
    constexpr span<int, 0> s{};

    TEST(s.front());
    TEST(s.back());
}