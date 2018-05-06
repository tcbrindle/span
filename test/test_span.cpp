
#define TCB_SPAN_NO_DEPRECATION_WARNINGS
#include <tcb/span.hpp>

#include <cassert>
#include <deque>
#include <initializer_list>
#include <vector>

#include "catch.hpp"

using tcb::span;

struct base {};
struct derived : base {};

TEST_CASE("default construction")
{
    static_assert(std::is_nothrow_default_constructible<span<int>>::value, "");
    static_assert(std::is_nothrow_default_constructible<span<int, 0>>::value,
                  "");
    static_assert(!std::is_default_constructible<span<int, 42>>::value, "");

    SECTION("dynamic size")
    {
        constexpr span<int> s{};
        static_assert(s.size() == 0, "");
        static_assert(s.data() == nullptr, "");
        // This causes an ICE on MSVC
#ifndef _MSC_VER
        static_assert(s.begin() == s.end(), "");
#else
        REQUIRE(s.begin() == s.end());
#endif
    }

    SECTION("fixed size")
    {
        constexpr span<int, 0> s{};
        static_assert(s.size() == 0, "");
        static_assert(s.data() == nullptr, "");
#ifndef _MSC_VER
        static_assert(s.begin() == s.end(), "");
#else
        REQUIRE(s.begin() == s.end());
#endif
    }
}

TEST_CASE("(pointer, length) construction")
{
    static_assert(std::is_constructible<span<int>, int*, int>::value, "");
    static_assert(std::is_constructible<span<const int>, int*, int>::value, "");
    static_assert(
        std::is_constructible<span<const int>, const int*, int>::value, "");

    static_assert(std::is_constructible<span<int, 42>, int*, int>::value, "");
    static_assert(std::is_constructible<span<const int, 42>, int*, int>::value,
                  "");
    static_assert(
        std::is_constructible<span<const int, 42>, const int*, int>::value, "");

    SECTION("dynamic size")
    {
        int arr[] = {1, 2, 3};
        span<int> s(arr, 3);

        REQUIRE(s.size() == 3);
        REQUIRE(s.data() == arr);
        REQUIRE(s.begin() == std::begin(arr));
        REQUIRE(s.end() == std::end(arr));
    }

    SECTION("fixed size")
    {
        int arr[] = {1, 2, 3};
        span<int, 3> s(arr, 3);

        REQUIRE(s.size() == 3);
        REQUIRE(s.data() == arr);
        REQUIRE(s.begin() == std::begin(arr));
        REQUIRE(s.end() == std::end(arr));
    }
}

TEST_CASE("(pointer, pointer) construction")
{
    static_assert(std::is_constructible<span<int>, int*, int*>::value, "");
    static_assert(!std::is_constructible<span<int>, float*, float*>::value, "");
    static_assert(std::is_constructible<span<int, 42>, int*, int*>::value, "");
    static_assert(!std::is_constructible<span<int, 42>, float*, float*>::value,
                  "");

    SECTION("dynamic size")
    {
        int arr[] = {1, 2, 3};
        span<int> s{arr, arr + 3};
        REQUIRE(s.size() == 3);
        REQUIRE(s.data() == arr);
        REQUIRE(s.begin() == std::begin(arr));
        REQUIRE(s.end() == std::end(arr));
    }

    SECTION("fixed size")
    {
        int arr[] = {1, 2, 3};
        span<int, 3> s{arr, arr + 3};
        REQUIRE(s.size() == 3);
        REQUIRE(s.data() == arr);
        REQUIRE(s.begin() == std::begin(arr));
        REQUIRE(s.end() == std::end(arr));
    }
}

TEST_CASE("C array construction")
{
    using int_array_t = int[3];
    using float_array_t = float[3];

    static_assert(std::is_nothrow_constructible<span<int>, int_array_t&>::value,
                  "");
    static_assert(!std::is_constructible<span<int>, int_array_t const&>::value,
                  "");
    static_assert(!std::is_constructible<span<int>, float_array_t>::value, "");

    static_assert(
        std::is_nothrow_constructible<span<const int>, int_array_t&>::value,
        "");
    static_assert(std::is_nothrow_constructible<span<const int>,
                                                int_array_t const&>::value,
                  "");
    static_assert(!std::is_constructible<span<const int>, float_array_t>::value,
                  "");

    static_assert(
        std::is_nothrow_constructible<span<int, 3>, int_array_t&>::value, "");
    static_assert(
        !std::is_constructible<span<int, 3>, int_array_t const&>::value, "");
    static_assert(!std::is_constructible<span<int, 3>, float_array_t&>::value,
                  "");

    static_assert(
        std::is_nothrow_constructible<span<const int, 3>, int_array_t&>::value,
        "");
    static_assert(std::is_nothrow_constructible<span<const int, 3>,
                                                int_array_t const&>::value,
                  "");
    static_assert(
        !std::is_constructible<span<const int, 3>, float_array_t>::value, "");

    static_assert(!std::is_constructible<span<int, 42>, int_array_t&>::value,
                  "");
    static_assert(
        !std::is_constructible<span<int, 42>, int_array_t const&>::value, "");
    static_assert(!std::is_constructible<span<int, 42>, float_array_t&>::value,
                  "");

    static_assert(
        !std::is_constructible<span<const int, 42>, int_array_t&>::value, "");
    static_assert(
        !std::is_constructible<span<const int, 42>, int_array_t const&>::value,
        "");
    static_assert(
        !std::is_constructible<span<const int, 42>, float_array_t&>::value, "");

    SECTION("non-const, dynamic size")
    {
        int arr[] = {1, 2, 3};
        span<int> s{arr};
        REQUIRE(s.size() == 3);
        REQUIRE(s.data() == arr);
        REQUIRE(s.begin() == std::begin(arr));
        REQUIRE(s.end() == std::end(arr));
    }

    SECTION("const, dynamic size")
    {
        int arr[] = {1, 2, 3};
        span<int const> s{arr};
        REQUIRE(s.size() == 3);
        REQUIRE(s.data() == arr);
        REQUIRE(s.begin() == std::begin(arr));
        REQUIRE(s.end() == std::end(arr));
    }

    SECTION("non-const, static size")
    {
        int arr[] = {1, 2, 3};
        span<int, 3> s{arr};
        REQUIRE(s.size() == 3);
        REQUIRE(s.data() == arr);
        REQUIRE(s.begin() == std::begin(arr));
        REQUIRE(s.end() == std::end(arr));
    }

    SECTION("const, dynamic size")
    {
        int arr[] = {1, 2, 3};
        span<int const, 3> s{arr};
        REQUIRE(s.size() == 3);
        REQUIRE(s.data() == arr);
        REQUIRE(s.begin() == std::begin(arr));
        REQUIRE(s.end() == std::end(arr));
    }
}

TEST_CASE("std::array construction")
{
    using int_array_t = std::array<int, 3>;
    using float_array_t = std::array<float, 3>;
    using zero_array_t = std::array<int, 0>;

    static_assert(std::is_nothrow_constructible<span<int>, int_array_t&>::value,
                  "");
    static_assert(!std::is_constructible<span<int>, int_array_t const&>::value,
                  "");
    static_assert(!std::is_constructible<span<int>, float_array_t>::value, "");

    static_assert(
        std::is_nothrow_constructible<span<const int>, int_array_t&>::value,
        "");
    static_assert(std::is_nothrow_constructible<span<const int>,
                                                int_array_t const&>::value,
                  "");
    static_assert(
        !std::is_constructible<span<const int>, float_array_t const&>::value,
        "");

    static_assert(
        std::is_nothrow_constructible<span<int, 3>, int_array_t&>::value, "");
    static_assert(
        !std::is_constructible<span<int, 3>, int_array_t const&>::value, "");
    static_assert(!std::is_constructible<span<int, 3>, float_array_t>::value,
                  "");

    static_assert(
        std::is_nothrow_constructible<span<const int, 3>, int_array_t&>::value,
        "");
    static_assert(std::is_nothrow_constructible<span<const int, 3>,
                                                int_array_t const&>::value,
                  "");
    static_assert(
        !std::is_constructible<span<const int, 3>, float_array_t const&>::value,
        "");

    static_assert(!std::is_constructible<span<int, 42>, int_array_t&>::value,
                  "");
    static_assert(
        !std::is_constructible<span<int, 42>, int_array_t const&>::value, "");
    static_assert(
        !std::is_constructible<span<int, 42>, float_array_t const&>::value, "");

    static_assert(
        !std::is_constructible<span<const int, 42>, int_array_t&>::value, "");
    static_assert(
        !std::is_constructible<span<const int, 42>, int_array_t const&>::value,
        "");
    static_assert(
        !std::is_constructible<span<const int, 42>, float_array_t&>::value, "");

    static_assert(std::is_constructible<span<int>, zero_array_t&>::value, "");
    static_assert(!std::is_constructible<span<int>, const zero_array_t&>::value,
                  "");
    static_assert(std::is_constructible<span<const int>, zero_array_t&>::value,
                  "");
    static_assert(
        std::is_constructible<span<const int>, const zero_array_t&>::value, "");

    static_assert(std::is_constructible<span<int, 0>, zero_array_t&>::value,
                  "");
    static_assert(
        !std::is_constructible<span<int, 0>, const zero_array_t&>::value, "");
    static_assert(
        std::is_constructible<span<const int, 0>, zero_array_t&>::value, "");
    static_assert(
        std::is_constructible<span<const int, 0>, const zero_array_t&>::value,
        "");

    SECTION("non-const, dynamic size")
    {
        int_array_t arr = {1, 2, 3};
        span<int> s{arr};
        REQUIRE(s.size() == 3);
        REQUIRE(s.data() == arr.data());
        REQUIRE(s.begin() == arr.data());
        REQUIRE(s.end() == arr.data() + 3);
    }

    SECTION("const, dynamic size")
    {
        int_array_t arr = {1, 2, 3};
        span<int const> s{arr};
        REQUIRE(s.size() == 3);
        REQUIRE(s.data() == arr.data());
        REQUIRE(s.begin() == arr.data());
        REQUIRE(s.end() == arr.data() + 3);
    }

    SECTION("non-const, static size")
    {
        int_array_t arr = {1, 2, 3};
        span<int, 3> s{arr};
        REQUIRE(s.size() == 3);
        REQUIRE(s.data() == arr.data());
        REQUIRE(s.begin() == arr.data());
        REQUIRE(s.end() == arr.data() + 3);
    }

    SECTION("const, dynamic size")
    {
        int_array_t arr = {1, 2, 3};
        span<int const, 3> s{arr};
        REQUIRE(s.size() == 3);
        REQUIRE(s.data() == arr.data());
        REQUIRE(s.begin() == arr.data());
        REQUIRE(s.end() == arr.data() + 3);
    }
}

TEST_CASE("Construction from other containers")
{
    using vec_t = std::vector<int>;
    using deque_t = std::deque<int>;

    static_assert(std::is_constructible<span<int>, vec_t&>::value, "");
    static_assert(!std::is_constructible<span<int>, const vec_t&>::value, "");
    static_assert(!std::is_constructible<span<int>, const deque_t&>::value, "");

    static_assert(std::is_constructible<span<const int>, vec_t&>::value, "");
    static_assert(std::is_constructible<span<const int>, const vec_t&>::value,
                  "");
    static_assert(
        !std::is_constructible<span<const int>, const deque_t&>::value, "");

    static_assert(std::is_constructible<span<int, 3>, vec_t&>::value, "");
    static_assert(!std::is_constructible<span<int, 3>, const vec_t&>::value,
                  "");
    static_assert(!std::is_constructible<span<int, 3>, const deque_t&>::value,
                  "");

    static_assert(std::is_constructible<span<const int, 3>, vec_t&>::value, "");
    static_assert(
        std::is_constructible<span<const int, 3>, const vec_t&>::value, "");
    static_assert(
        !std::is_constructible<span<const int, 3>, const deque_t&>::value, "");

    SECTION("non-const, dynamic size")
    {
        vec_t arr = {1, 2, 3};
        span<int> s{arr};
        REQUIRE(s.size() == 3);
        REQUIRE(s.data() == arr.data());
        REQUIRE(s.begin() == arr.data());
        REQUIRE(s.end() == arr.data() + 3);
    }

    SECTION("const, dynamic size")
    {
        vec_t arr = {1, 2, 3};
        span<int const> s{arr};
        REQUIRE(s.size() == 3);
        REQUIRE(s.data() == arr.data());
        REQUIRE(s.begin() == arr.data());
        REQUIRE(s.end() == arr.data() + 3);
    }

    SECTION("non-const, static size")
    {
        vec_t arr = {1, 2, 3};
        span<int, 3> s{arr};
        REQUIRE(s.size() == 3);
        REQUIRE(s.data() == arr.data());
        REQUIRE(s.begin() == arr.data());
        REQUIRE(s.end() == arr.data() + 3);
    }

    SECTION("const, dynamic size")
    {
        vec_t arr = {1, 2, 3};
        span<int const, 3> s{arr};
        REQUIRE(s.size() == 3);
        REQUIRE(s.data() == arr.data());
        REQUIRE(s.begin() == arr.data());
        REQUIRE(s.end() == arr.data() + 3);
    }
}

TEST_CASE("construction from spans of different size")
{
    using zero_span = span<int, 0>;
    using zero_const_span = span<const int, 0>;
    using big_span = span<int, 1000000>;
    using big_const_span = span<const int, 1000000>;
    using dynamic_span = span<int>;
    using dynamic_const_span = span<const int>;

    static_assert(std::is_trivially_copyable<zero_span>::value, "");
    static_assert(std::is_trivially_move_constructible<zero_span>::value, "");
    static_assert(!std::is_constructible<zero_span, zero_const_span>::value,
                  "");
    static_assert(!std::is_constructible<zero_span, big_span>::value, "");
    static_assert(!std::is_constructible<zero_span, big_const_span>::value, "");
    static_assert(!std::is_constructible<zero_span, dynamic_span>::value, "");
    static_assert(!std::is_constructible<zero_span, dynamic_const_span>::value,
                  "");

    static_assert(
        std::is_nothrow_constructible<zero_const_span, zero_span>::value, "");
    static_assert(std::is_trivially_copyable<zero_const_span>::value, "");
    static_assert(std::is_trivially_move_constructible<zero_const_span>::value,
                  "");
    static_assert(!std::is_constructible<zero_const_span, big_span>::value, "");
    static_assert(
        !std::is_constructible<zero_const_span, big_const_span>::value, "");
    static_assert(!std::is_constructible<zero_const_span, dynamic_span>::value,
                  "");
    static_assert(
        !std::is_constructible<zero_const_span, dynamic_const_span>::value, "");

    static_assert(!std::is_constructible<big_span, zero_span>::value, "");
    static_assert(!std::is_constructible<big_span, zero_const_span>::value, "");
    static_assert(std::is_trivially_copyable<big_span>::value, "");
    static_assert(std::is_trivially_move_constructible<big_span>::value, "");
    static_assert(!std::is_constructible<big_span, big_const_span>::value, "");
    static_assert(!std::is_constructible<big_span, dynamic_span>::value, "");
    static_assert(!std::is_constructible<big_span, dynamic_const_span>::value,
                  "");

    static_assert(!std::is_constructible<big_const_span, zero_span>::value, "");
    static_assert(
        !std::is_constructible<big_const_span, zero_const_span>::value, "");
    static_assert(std::is_trivially_copyable<big_const_span>::value, "");
    static_assert(std::is_trivially_move_constructible<big_const_span>::value,
                  "");
    static_assert(
        std::is_nothrow_constructible<big_const_span, big_span>::value, "");
    static_assert(!std::is_constructible<big_const_span, dynamic_span>::value,
                  "");
    static_assert(
        !std::is_constructible<big_const_span, dynamic_const_span>::value, "");

    static_assert(std::is_nothrow_constructible<dynamic_span, zero_span>::value,
                  "");
    static_assert(!std::is_constructible<dynamic_span, zero_const_span>::value,
                  "");
    static_assert(std::is_nothrow_constructible<dynamic_span, big_span>::value,
                  "");
    static_assert(!std::is_constructible<dynamic_span, big_const_span>::value,
                  "");
    static_assert(std::is_trivially_copyable<dynamic_span>::value, "");
    static_assert(std::is_trivially_move_constructible<dynamic_span>::value,
                  "");
    static_assert(
        !std::is_constructible<dynamic_span, dynamic_const_span>::value, "");

    static_assert(
        std::is_nothrow_constructible<dynamic_const_span, zero_span>::value,
        "");
    static_assert(std::is_nothrow_constructible<dynamic_const_span,
                                                zero_const_span>::value,
                  "");
    static_assert(
        std::is_nothrow_constructible<dynamic_const_span, big_span>::value, "");
    static_assert(std::is_nothrow_constructible<dynamic_const_span,
                                                big_const_span>::value,
                  "");
    static_assert(
        std::is_nothrow_constructible<dynamic_const_span, dynamic_span>::value,
        "");
    static_assert(std::is_trivially_copyable<dynamic_const_span>::value, "");
    static_assert(
        std::is_trivially_move_constructible<dynamic_const_span>::value, "");

    constexpr zero_const_span s0{};
    constexpr dynamic_const_span d{s0};

    static_assert(d.size() == 0, "");
    static_assert(d.data() == nullptr, "");
#ifndef _MSC_VER
    static_assert(d.begin() == d.end(), "");
#else
    REQUIRE(d.begin() == d.end());
#endif
}

TEST_CASE("member subview operations")
{
    SECTION("first<N>")
    {
        int arr[] = {1, 2, 3, 4, 5};
        span<int, 5> s{arr};
        auto f = s.first<3>();

        static_assert(std::is_same<decltype(f), span<int, 3>>::value, "");
        REQUIRE(f.size() == 3);
        REQUIRE(f.data() == arr);
        REQUIRE(f.begin() == arr);
        REQUIRE(f.end() == arr + 3);
    }

    SECTION("last<N>")
    {
        int arr[] = {1, 2, 3, 4, 5};
        span<int, 5> s{arr};
        auto l = s.last<3>();

        static_assert(std::is_same<decltype(l), span<int, 3>>::value, "");
        REQUIRE(l.size() == 3);
        REQUIRE(l.data() == arr + 2);
        REQUIRE(l.begin() == arr + 2);
        REQUIRE(l.end() == std::end(arr));
    }

    SECTION("subspan<N>")
    {
        int arr[] = {1, 2, 3, 4, 5};
        span<int, 5> s{arr};
        auto ss = s.subspan<1, 2>();

        static_assert(std::is_same<decltype(ss), span<int, 2>>::value, "");
        REQUIRE(ss.size() == 2);
        REQUIRE(ss.data() == arr + 1);
        REQUIRE(ss.begin() == arr + 1);
        REQUIRE(ss.end() == arr + 1 + 2);
    }

    SECTION("first(n)")
    {
        int arr[] = {1, 2, 3, 4, 5};
        span<int, 5> s{arr};
        auto f = s.first(3);

        static_assert(std::is_same<decltype(f), span<int>>::value, "");
        REQUIRE(f.size() == 3);
        REQUIRE(f.data() == arr);
        REQUIRE(f.begin() == arr);
        REQUIRE(f.end() == arr + 3);
    }

    SECTION("last(n)")
    {
        int arr[] = {1, 2, 3, 4, 5};
        span<int, 5> s{arr};
        auto l = s.last(3);

        static_assert(std::is_same<decltype(l), span<int>>::value, "");
        REQUIRE(l.size() == 3);
        REQUIRE(l.data() == arr + 2);
        REQUIRE(l.begin() == arr + 2);
        REQUIRE(l.end() == std::end(arr));
    }

    SECTION("subspan(n)")
    {
        int arr[] = {1, 2, 3, 4, 5};
        span<int, 5> s{arr};
        auto ss = s.subspan(1, 2);

        static_assert(std::is_same<decltype(ss), span<int>>::value, "");
        REQUIRE(ss.size() == 2);
        REQUIRE(ss.data() == arr + 1);
        REQUIRE(ss.begin() == arr + 1);
        REQUIRE(ss.end() == arr + 1 + 2);
    }

    // TODO: Test all the dynamic subspan possibilities
}

TEST_CASE("span observers")
{
    // We already use this everywhere, but whatever
    constexpr span<int, 0> empty{};
    static_assert(empty.size() == 0, "");
    static_assert(empty.empty(), "");

    constexpr int arr[] = {1, 2, 3};
    static_assert(span<const int>{arr}.size() == 3, "");
    static_assert(!span<const int>{arr}.empty(), "");
}

TEST_CASE("span element access")
{
    constexpr int arr[] = {1, 2, 3};
    span<const int> s{arr};

    REQUIRE(s[0] == arr[0]);
    REQUIRE(s[1] == arr[1]);
    REQUIRE(s[2] == arr[2]);

    REQUIRE(s(0) == arr[0]);
    REQUIRE(s(1) == arr[1]);
    REQUIRE(s(2) == arr[2]);

    REQUIRE(s.at(0) == arr[0]);
    REQUIRE(s.at(1) == arr[1]);
    REQUIRE(s.at(2) == arr[2]);
    REQUIRE_THROWS_AS(s.at(42), std::out_of_range);
}

TEST_CASE("span iterator support")
{
    {
        std::vector<int> vec;
        span<int> s{vec};
        std::sort(s.begin(), s.end());
        REQUIRE(std::is_sorted(vec.cbegin(), vec.cend()));
    }

    {
        const std::vector<int> vec{1, 2, 3};
        span<const int> s{vec};
        REQUIRE(std::equal(s.rbegin(), s.rend(), vec.crbegin()));
    }
}

TEST_CASE("nonmember first<N>()")
{
    {
        int arr[] = {1, 2, 3, 4, 5};
        auto s = tcb::first<3>(arr);
        static_assert(std::is_same<decltype(s), span<int, 3>>::value, "");

        REQUIRE(s.size() == 3);
        REQUIRE(s.data() == arr);
        REQUIRE(s.begin() == arr);
        REQUIRE(s.end() == arr + 3);
    }

    {
        const int arr[] = {1, 2, 3, 4, 5};
        auto s = tcb::first<3>(arr);
        static_assert(std::is_same<decltype(s), span<const int, 3>>::value, "");

        REQUIRE(s.size() == 3);
        REQUIRE(s.data() == arr);
        REQUIRE(s.begin() == arr);
        REQUIRE(s.end() == arr + 3);
    }

    {
        std::array<int, 5> arr = {1, 2, 3, 4, 5};
        auto s = tcb::first<3>(arr);
        static_assert(std::is_same<decltype(s), span<int, 3>>::value, "");

        REQUIRE(s.size() == 3);
        REQUIRE(s.data() == arr.data());
        REQUIRE(s.begin() == arr.data());
        REQUIRE(s.end() == arr.data() + 3);
    }

    {
        const std::array<int, 5> arr = {1, 2, 3, 4, 5};
        auto s = tcb::first<3>(arr);
        static_assert(std::is_same<decltype(s), span<const int, 3>>::value, "");

        REQUIRE(s.size() == 3);
        REQUIRE(s.data() == arr.data());
        REQUIRE(s.begin() == arr.data());
        REQUIRE(s.end() == arr.data() + 3);
    }

    {
        std::vector<int> vec = {1, 2, 3, 4, 5};
        auto s = tcb::first<3>(vec);
        static_assert(std::is_same<decltype(s), span<int, 3>>::value, "");

        REQUIRE(s.size() == 3);
        REQUIRE(s.data() == vec.data());
        REQUIRE(s.begin() == vec.data());
        REQUIRE(s.end() == vec.data() + 3);
    }

    {
        const std::vector<int> vec = {1, 2, 3, 4, 5};
        auto s = tcb::first<3>(vec);
        static_assert(std::is_same<decltype(s), span<const int, 3>>::value, "");

        REQUIRE(s.size() == 3);
        REQUIRE(s.data() == vec.data());
        REQUIRE(s.begin() == vec.data());
        REQUIRE(s.end() == vec.data() + 3);
    }
}

TEST_CASE("make_span()")
{
    {
        int arr[3] = {1, 2, 3};
        auto s = tcb::make_span(arr);
        static_assert(std::is_same<decltype(s), span<int, 3>>::value, "");
        REQUIRE(s.data() == arr);
        REQUIRE(s.size() == 3);
    }

    {
        const int arr[3] = {1, 2, 3};
        auto s = tcb::make_span(arr);
        static_assert(std::is_same<decltype(s), span<const int, 3>>::value, "");
        REQUIRE(s.data() == arr);
        REQUIRE(s.size() == 3);
    }

    {
        std::array<int, 3> arr = {1, 2, 3};
        auto s = tcb::make_span(arr);
        static_assert(std::is_same<decltype(s), span<int, 3>>::value, "");
        REQUIRE(s.data() == arr.data());
        REQUIRE(s.size() == arr.size());
    }

    {
        const std::array<int, 3> arr = {1, 2, 3};
        auto s = tcb::make_span(arr);
        static_assert(std::is_same<decltype(s), span<const int, 3>>::value, "");
        REQUIRE(s.data() == arr.data());
        REQUIRE(s.size() == 3);
    }

    {
        std::vector<int> arr = {1, 2, 3};
        auto s = tcb::make_span(arr);
        static_assert(std::is_same<decltype(s), span<int>>::value, "");
        REQUIRE(s.data() == arr.data());
        REQUIRE(s.size() == arr.size());
    }

    {
        const std::vector<int> arr = {1, 2, 3};
        auto s = tcb::make_span(arr);
        static_assert(std::is_same<decltype(s), span<const int>>::value, "");
        REQUIRE(s.data() == arr.data());
        REQUIRE(s.size() == arr.size());
    }
}