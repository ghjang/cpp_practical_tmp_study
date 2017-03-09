#include "catch.hpp"

#include <cstddef>

#include "02_type_is.h"


//==============================================================================
// numeric calculation
//==============================================================================

// constexpr recursive_factorial function for C++11
constexpr std::size_t recursive_factorial(std::size_t i)
{
    return (0 == i || 1 == i) ? 1
                              : (i * recursive_factorial(i - 1));
}

// constexpr iterative_factorial function for C++14
constexpr std::size_t iterative_factorial(std::size_t i)
{
    std::size_t val = 1;
    for ( ; i; --i) {
        val *= i;
    }
    return val;
}

// factorial_t metafunction
template <std::size_t i>
struct factorial_t
{
    constexpr static auto value = factorial_t<i - 1>::value * i;
};

template <>
struct factorial_t<0>
{
    constexpr static auto value = 1;
};

template <>
struct factorial_t<1>
{
    constexpr static auto value = 1;
};


TEST_CASE("factorial metafunction", "[tmp]")
{
    //==== recursive factorial
    auto rf = recursive_factorial(5);
    //static_assert(rf == 1 * 2 * 3 * 4 * 5);   // expected compiler error
    REQUIRE(rf == 1 * 2 * 3 * 4 * 5);           // need to check at runtime

    constexpr auto crf = recursive_factorial(5);
    static_assert(crf == 1 * 2 * 3 * 4 * 5);
    static_assert(recursive_factorial(5) == 1 * 2 * 3 * 4 * 5);

    //==== iterative factorial
    auto iterf = iterative_factorial(5);
    //static_assert(iterf == 1 * 2 * 3 * 4 * 5);    // expected compiler error
    REQUIRE(iterf == 1 * 2 * 3 * 4 * 5);            // need to check at runtime

    constexpr auto citerf = iterative_factorial(5);
    static_assert(citerf == 1 * 2 * 3 * 4 * 5);
    static_assert(iterative_factorial(5) == 1 * 2 * 3 * 4 * 5);

    //==== template metafunction factorial
    static_assert(factorial_t<0>::value == 1);
    static_assert(factorial_t<5>::value == 1 * 2 * 3 * 4 * 5);

    // You can call constexpr function inside the template argument list.
    static_assert(
        factorial_t<
            iterative_factorial(3)  // <== 6
        >::value == 1 * 2 * 3 * 4 * 5 * 6
    );
}

//==============================================================================
// type calculation
//==============================================================================

// runtime function f
template <typename Arg>
Arg f(Arg a)
{
    return a;           // returns a value
}

// compile-time function F
template <typename Arg>
struct F
{
    using type = Arg;   // returns a type
};

// using type alias for convenience
template <typename Arg>
using f_t = typename F<Arg>::type;

TEST_CASE("type calculation metafunction", "[tmp]")
{
    auto a = f(100);

    F<int>::type b = 200;

    f_t<int> c = 300;
}

//==============================================================================
template <typename T>
struct add_pointer
{
    using type = T *;
};

template <typename T>
struct add_pointer_fwd
            : type_is<T *>  // metafunction forwarding
{ };

template <typename T>
using add_pointer_fwd_t = typename add_pointer_fwd<T>::type;

TEST_CASE("metafunction forwarding and type_is utility metafunction", "[tmp]")
{
    // add_pointer metafunction call
    using my_ptr = add_pointer<int>::type;
    int i = 100;
    my_ptr ptrI = &i;

    using my_ptr_ptr = add_pointer<int *>::type;
    my_ptr_ptr ptrPtrI = &ptrI;

    // add_pointer_fwd metafunction call
    using my_another_ptr = add_pointer_fwd<double>::type;
    double d = 100.;
    my_another_ptr ptrD = &d;

    // add_pointer_fwd_t type alias call
    using my_another_ptr1 = add_pointer_fwd_t<double>;
    my_another_ptr1 ptrD1 = &d;
}
