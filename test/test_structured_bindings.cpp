
#include <tcb/span.hpp>

#include "catch.hpp"

using static_span_t = tcb::span<int, 3>;
using dynamic_span_t = tcb::span<int>;

static_assert(std::tuple_size_v<static_span_t> == static_span_t::extent);
static_assert(!tcb::detail::is_complete<std::tuple_size<dynamic_span_t>>::value);

TEST_CASE("Structured bindings")
{
    // C++, why you no let me do constexpr structured bindings?

    int arr[] = {1, 2, 3};

    auto& [a1, a2, a3] = arr;
    auto&& [s1, s2, s3] = tcb::make_span(arr);

    REQUIRE(a1 == s1);
    REQUIRE(a2 == s2);
    REQUIRE(a3 == s3);

    a1 = 99;
    REQUIRE(s1 == 99);

    s2 = 100;
    REQUIRE(a2 == 100);
}
