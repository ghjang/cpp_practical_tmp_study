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

// primary template
//      ==> default pattern matching
//      ==> 'else' branch
template <std::size_t i>
struct factorial_t
{
    //  ==> recursive 'loop'
    constexpr static auto value = factorial_t<i - 1>::value * i;
};

// full template specialization
//      ==> base case pattern matching
//      ==> 'if' branch
template <>
struct factorial_t<0>
{
    constexpr static auto value = 1;
};

// another template specialization
//      ==> another base case pattern matching
//      ==> another 'if' branch, somewhat like 'else if'
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


template <std::size_t i>
struct binary
{
    static_assert(i % 10 == 0 || i % 10 == 1, "i should have 0 or 1 digit values.");

    constexpr static auto value = (binary<i / 10>::value * 2)
                                        + (i % 10);
};

template <>
struct binary<0>
{
    constexpr static auto value = 0;
};

TEST_CASE("binary metafunction", "[tmp]")
{
    static_assert(binary<1010>::value == 10);
    static_assert(binary<10000>::value == 16);

    // NOTE: used double parenthesis due to linker error.
    //          catch.hpp bug??
    REQUIRE((binary<1011>::value == 11));

    // NOTE: this is an expected compile error.
    //          '2' is not a binary digit number.
    //REQUIRE((binary<1210>::value == 0));

    // NOTE: C++14 has binary-literal integer.
    auto i = 0b10000;
    REQUIRE(i == 16);
    REQUIRE(0b1111 == 15);
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

TEST_CASE("add_pointer metafunction", "[tmp]")
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


template <typename T>
struct add_const_pointer
            : add_pointer_fwd<T const>
{ };

template <typename T>
using add_const_pointer_t = typename add_const_pointer<T>::type;

TEST_CASE("add_const_pointer metafunction", "[tmp]")
{
    using my_top_level_const_ptr = add_const_pointer_t<int>;
    using my_t = add_pointer_fwd_t<my_top_level_const_ptr>;

    // int const * *
    my_t t = nullptr;
    int const * * i = nullptr;
    int const * * * j = nullptr;

    t = i;  // OK. they're same type.
    //t = j;  // compile error
}
