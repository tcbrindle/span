
`std::span` implementation for C++11 and later
==============================================

This repository contains a single-header implementation of C++20's `std::span`,
conforming to [P0122R7](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2018/p0122r7.pdf).
It is compatible with C++11, but will use newer language features if they
are available.

It differs from the implementation in the [Microsoft GSL](https://github.com/Microsoft/GSL/)
in that it is single-header and does not depend on any other GSL facilities. It
also works with C++11, while the GSL version requires C++14. Finally, it provides
various extensions (detailed below) which are currently not part of the
standard specification.

Usage
-----

The recommended way to use the implementation simply copy the file `span.hpp`
from `include/tcb/` into your own sources and `#include` it like
any other header. By default, it lives in namespace `tcb`, but this can be
customised by setting the macro `TCB_SPAN_NAMESPACE_NAME` to an appropriate string
before `#include`-ing the header -- or simply edit the source code.

The rest of the repository contains testing machinery, and is not required for
use.


Implementation Notes
--------------------

### Bounds Checking ###

This implementation of `span` includes optional bounds checking, which is handled
either by throwing an exception or by calling `std::terminate()`.

The default behaviour with C++14 and later is to check the macro `NDEBUG`:
if this is set, bounds checking is disabled. Otherwise, `std::terminate()` will
be called if there is a precondition violation (i.e. the same behaviour as
`assert()`). If you wish to terminate on errors even if `NDEBUG` is set, define
the symbol `TCB_SPAN_TERMINATE_ON_CONTRACT_VIOLATION` before `#include`-ing the
header.

Alternatively, if you want to throw on a contract violation, define
`TCB_SPAN_THROW_ON_CONTRACT_VIOLATION`. This will throw an exception of an
implementation-defined type (deriving from `std::logic_error`), allowing
cleanup to happen. Note that defining this symbol will cause the checks to be
run even if `NDEBUG` is set.

Lastly, if you wish to disable contract checking even in debug builds,
`#define TCB_SPAN_NO_CONTRACT_CHECKING`.

Under C++11, due to the restrictions on `constexpr` functions, contract checking
is disabled by default even if `NDEBUG` is not set. You can change this by
defining either of the above symbols, but this will result in most of `span`'s
interface becoming non-`constexpr`.

### `constexpr` ###

This implementation is fully `constexpr` under C++17 and later. Under earlier
versions, it is "as `constexpr` as possible".

Note that even in C++17, it is generally not possible to declare a `span`
as non-default constructed `constexpr` variable, for the same reason that you
cannot form a `constexpr` pointer to a value: it involves taking the address of
a compile-time variable in a way that would be visible at run-time.
You can however use a `span` freely in a `constexpr` function. For example:

```cpp
// Okay, even in C++11
constexpr std::ptrdiff_t get_span_size(span<const int> span)
{
    return span.size();
}

constexpr int arr[] = {1, 2, 3};
constexpr auto size = get_span_size(arr); // Okay
constexpr span<const int> span{arr}; // ERROR -- not a constant expression
constexpr const int* p = arr; // ERROR -- same
```

Constructor deduction guides are provided if the compiler supports them. For
older compilers, a `make_span()` function is provided as an extension
which uses the same logic.

Extensions
----------

This implementation provides several extensions to the current specification.

 * Added `make_span()` factory functions

   These perform the same function as the C++17 deduction guides, and allow you
   to construct a `span` without needing to specify the template arguments.
   For example:

   ```cpp
   constexpr int c_array[] = {1, 2, 3};
   std::array<int, 3> std_array{1, 2, 3};
   const std::vector<int> vec{1, 2, 3};

   auto s1 = make_span(c_array);   // returns span<const int, 3>
   auto s2 = make_span(std_array); // returns span<int, 3>
   auto s3 = make_span(vec);       // returns span<const int, dynamic_extent>
   ```

 * Added `front()` and `back()` member functions

   These are provided for consistency with the standard containers. They
   return references to the first and last elements of the `span` respectively.
   The effect of calling these functions on an empty `span` is undefined.

 * Added bounds-checking `at(index_type i)` member function

   Again, added for consistency with standard constainers.
   Returns the `i`th member of the `span`. Throws `std::out_of_range` if `i < 0`
   or `i >= span.size()`.

   If you wish to disable exceptions, define the macro `TCB_SPAN_NO_EXCEPTIONS`
   before `#include`-ing `span.hpp`. The `at()` member function will still be
   available, but no bounds checking will be performed. The header will also try
   to detect `-fno-exceptions` and the like and disable bounds checking in this
   case.

 * Adds non-member subview operations, `first()` `last()` and `subspan()`

   The member versions of these functions can require the awkward "dot template"
   syntax to be used when the `span` is a dependent type, for example:

   ```cpp
   template <typename T>
   span<T, 3> get_first_three(span<T> s)
   {
       return s.template first<3>(); // Urgh!
   }
   ```

   Making these functions non-members avoids this syntactic ugliness. In addition,
   it allows forming subviews of containers directly without constructing an
   intermediate `span`, for example:

   ```cpp
   std::vector<int> vec{1, 2, 3, 4, 5};

   auto ugly = span{vec}.first<3>();
   auto nicer = first<3>(vec);
   ```

 * `operator()` is `[[deprecated]]`

   The currently-specified version of `span` includes an overload of `operator()`,
   duplicating the functionality of `operator[]`. This rather odd addition is
   marked as `[[deprecated]]` in this version by default. You can disable this
   warning by defining the macro `TCB_SPAN_NO_DEPRECATION_WARNINGS` before
   `#include`-ing `span.hpp`, or remove the definition entirely by defining
   `TCB_SPAN_NO_FUNCTION_CALL_OPERATOR`.

 * Add structured bindings support for fixed-size `span`s

   This implementation includes a `get()` non-member function and specialisations of
   `std::tuple_size` and `std::tuple_element`, which allows `span`s to be used
   with C++17 structured bindings.

   Note that dynamically-sized `span`s cannot be decomposed, because the size is
   not known at run time.

To disable all extensions, define the macro `TCB_SPAN_STD_COMPLIANT_MODE`. This
will also disable deprecation warnings.

