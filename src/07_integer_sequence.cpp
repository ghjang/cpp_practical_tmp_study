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


// use std::array instead in C++17
template <typename T, std::size_t N>
struct const_expr_array
{
    constexpr std::size_t size() const
    { return N; }

    constexpr T const& operator [] (std::size_t i) const
    { return array_[i]; }

    constexpr T & operator [] (std::size_t i)
    { return array_[i]; }

    T array_[N]{};
};

struct identity_next
{
    template <typename T>
    constexpr static auto next(T t)
    { return t; }
};

struct next_square_number
{
    template <typename T>
    constexpr static auto next(T t)
    { return t * t; }
};

struct next_even_number
{
    template <typename T>
    constexpr static auto next(T t)
    { return t * 2; }
};

template <std::size_t n, typename F = identity_next>
struct index_sequence_generator
{
    constexpr static auto make()
    {
        const_expr_array<std::size_t, n> indices{};
        for (std::size_t i = 0; i < n; ++i) {
            // NOTE: C++14 std::array doesn't support this.
            indices[i] = F::next(i);
        }
        return indices;
    }

    constexpr static auto indices_ = make();

    template <std::size_t... i>
    static
    std::index_sequence<indices_[i]...>
    as_index_sequence(std::index_sequence<i...>);

    using type = decltype(as_index_sequence(std::make_index_sequence<n>{}));
};

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

    using index_seq_t = index_sequence_generator<5, next_even_number>::type;
    
    static_assert(std::size(factorials<index_seq_t>) == 5);

    static_assert(factorials<index_seq_t>[0] == 1);                             // 0!
    static_assert(factorials<index_seq_t>[1] == 1 * 2);                         // 2!
    static_assert(factorials<index_seq_t>[2] == 1 * 2 * 3 * 4);                 // 4!
    static_assert(factorials<index_seq_t>[3] == 1 * 2 * 3 * 4 * 5 * 6);         // 6!
    static_assert(factorials<index_seq_t>[4] == 1 * 2 * 3 * 4 * 5 * 6 * 7 * 8); // 8!
}
