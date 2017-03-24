#include "catch.hpp"

#include <type_traits>
#include <utility>
#include <iterator>
#include <array>

#include "02_type.h"


//==============================================================================
// C++14 std::integer_sequence

TEST_CASE("std::integer_sequence basic", "[tmp]")
{
    using int_seq_0 = std::integer_sequence<std::size_t, 0, 1, 2, 3, 4>;
    using int_seq_1 = std::make_integer_sequence<std::size_t, 5>;
    static_assert(std::is_same<int_seq_0, int_seq_1>());

    using index_seq_0 = std::index_sequence<0, 1, 2, 3, 4>;
    using index_seq_1 = std::make_index_sequence<5>;
    static_assert(std::is_same<index_seq_0, index_seq_1>());

    static_assert(std::is_same<int_seq_0, index_seq_1>());
}

//==============================================================================
// factorial example revisited.

// constexpr iterative_factorial function for C++14
constexpr static std::size_t iterative_factorial(std::size_t i)
{
    std::size_t val = 1;
    for ( ; i; --i) {
        val *= i;
    }
    return val;
}

// C++14 variable template
template <typename IndexSeq>
constexpr static std::size_t factorials[] = { };

template <typename T, std::size_t... i>
constexpr static T factorials<std::integer_sequence<T, i...>>[sizeof...(i)] = {
                        iterative_factorial(i)...
};

template <std::size_t n>
constexpr static std::add_lvalue_reference_t<
                        decltype(factorials<std::make_index_sequence<n>>)
                 >
fact = factorials<std::make_index_sequence<n>>;

TEST_CASE("factorial table with std::index_sequence", "[tmp]")
{
    static_assert(
        std::size(factorials<std::make_index_sequence<5>>) == 5
    );

    static_assert(std::size(fact<5>) == 5);
    static_assert(fact<5>[0] == 1);
    static_assert(fact<5>[4] == 1 * 2 * 3 * 4);

    for (auto v : fact<5>) {
        // ...
    }
}


#include "08_index_sequence_generator.h"

TEST_CASE("factorial table with arbitrary index sequence", "[tmp]")
{
    static_assert(
        std::is_same<
                std::make_index_sequence<5>,
                index_sequence_generator<5>::type
        >()
    );

    static_assert(
        std::is_same<
                std::index_sequence<0, 1, 4, 9, 16>,
                index_sequence_generator<5, next_square_number>::type
        >()
    );

    static_assert(
        std::is_same<
                std::index_sequence<0, 2, 4, 6, 8>,
                index_sequence_generator<5, next_even_number>::type
        >()
    );

    static_assert(
        std::is_same<
                std::index_sequence<1, 3, 5, 7, 9>,
                index_sequence_generator<5, next_odd_number>::type
        >()
    );

    using index_seq_t = index_sequence_generator<5, next_even_number>::type;
    
    static_assert(std::size(factorials<index_seq_t>) == 5);

    static_assert(factorials<index_seq_t>[0] == 1);                             // 0!
    static_assert(factorials<index_seq_t>[1] == 1 * 2);                         // 2!
    static_assert(factorials<index_seq_t>[2] == 1 * 2 * 3 * 4);                 // 4!
    static_assert(factorials<index_seq_t>[3] == 1 * 2 * 3 * 4 * 5 * 6);         // 6!
    static_assert(factorials<index_seq_t>[4] == 1 * 2 * 3 * 4 * 5 * 6 * 7 * 8); // 8!
}


//==============================================================================
// more about constexpr array

// An array can be used in a constexpr function.
constexpr static int my_constexpr_func_with_array()
{
    int arr[5] = { 1, 2, 3, 4, 5 };
    for (int i = 0; i < std::size(arr); ++i) {
        ++arr[i];   // You can modify array values.
    }
    int sum = 0;
    for (int i = 0; i < std::size(arr); ++i) {
        sum += arr[i];
    }
    return sum;
}

TEST_CASE("array in a constexpr function", "[tmp]")
{
    static_assert(my_constexpr_func_with_array() == 20);
}

// But, you can't return an array itself in C++.
// my_constexpr_func_returing_array results in a compile error.
/*
constexpr static int [5] my_constexpr_func_returing_array()
{
    int arr[5] = { 1, 2, 3, 4, 5 };
    return arr;
}
*/

// This is also a compile error.
/*
constexpr static decltype(auto) my_constexpr_func_returing_array_1()
{
    int arr[5] = { 1, 2, 3, 4, 5 };
    return arr;
}
*/

// This is compiled OK due to the array-to-pointer decay.
// But, returning a local array pointer is a common programming error.
/*
constexpr static auto my_constexpr_func_returing_array_2()
{
    int arr[5] = { 1, 2, 3, 4, 5 };
    return arr;
}
*/

// 'static variable not permitted in a constexpr function'
/*
constexpr static auto my_constexpr_func_returing_array_3()
{
    static int arr[5] = { 1, 2, 3, 4, 5 };
    return arr;
}
*/

// Finally, this is OK. You can return a wrapped array!!
constexpr static auto my_constexpr_func_returning_array_4()
{
    const_expr_array<std::size_t, 5> arr = { 1, 2, 3, 4, 5, };
    return arr;
}

constexpr static auto my_array_sum()
{
    auto arr = my_constexpr_func_returning_array_4();
    for (int i = 0; i < arr.size(); ++i) {
        ++arr[i];
    }
    int sum = 0;
    for (int i = 0; i < arr.size(); ++i) {
        sum += arr[i];
    }
    return sum;
}

TEST_CASE("constexpr function returning a wrapped array", "[tmp]")
{
    static_assert(my_array_sum() == 20);
}

#if __clang_major__ >= 4    // if C++17 std::array is available,
constexpr static auto my_constexpr_func_returning_array_5()
{
    std::array<std::size_t, 5> arr = { 1, 2, 3, 4, 5 };
    return arr;
}

constexpr static auto my_array_sum_1()
{
    auto arr = my_constexpr_func_returning_array_5();
    for (int i = 0; i < arr.size(); ++i) {
        ++arr[i];   // NOTE: std::array allows this data writing in C++17,
                    //          but not in C++14.
    }
    int sum = 0;
    for (auto val : arr) {
        sum += val;
    }
    return sum;
}

TEST_CASE("constexpr function returning std::array", "[tmp]")
{
    static_assert(my_array_sum_1() == 20);
}
#endif

//==============================================================================
// generating integer sequence in a old way.

template <typename T, T... i>
struct my_integer_sequence
{ };


template <typename T, T init, T n, typename Seq = my_integer_sequence<T>>
struct my_integer_sequence_impl;

template <typename T, T n, T... i>
struct my_integer_sequence_impl<T, n, n, my_integer_sequence<T, i...>>
        : type_is<my_integer_sequence<T, i...>>
{ };

template <typename T, T init, T n, T... i>
struct my_integer_sequence_impl<T, init, n, my_integer_sequence<T, i...>>
        : my_integer_sequence_impl<
                T,
                init + 1,
                n,
                my_integer_sequence<T, i..., init>
          >
{ };


template <typename T, T n>
using make_my_integer_sequence = typename my_integer_sequence_impl<T, 0, n>::type;


TEST_CASE("make_my_integer_sequence", "[tmp]")
{
    static_assert(
        std::is_same<
            my_integer_sequence<int, 0, 1, 2, 3, 4>,
            make_my_integer_sequence<int, 5>
        >()
    );
}
