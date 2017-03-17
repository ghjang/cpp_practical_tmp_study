#include "catch.hpp"

#include <type_traits>
#include <utility>
#include <iterator>
#include <array>

//==============================================================================
// C++14 std::integer_sequence

TEST_CASE("std::integer_sequence", "[tmp]")
{
    using int_seq_0 = std::integer_sequence<std::size_t, 0, 1, 2, 3, 4>;
    using int_seq_1 = std::make_integer_sequence<std::size_t, 5>;
    static_assert(std::is_same<int_seq_0, int_seq_1>());

    using index_seq_0 = std::index_sequence<0, 1, 2, 3, 4>;
    using index_seq_1 = std::make_index_sequence<5>;
    static_assert(std::is_same<index_seq_0, index_seq_1>());

    static_assert(std::is_same<int_seq_0, index_seq_1>());
}


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

//constexpr auto make_factorial()
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
