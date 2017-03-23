#ifndef CPP_TMP_STUDY_INDEX_SEQUENCE_GENERATOR_H
#define CPP_TMP_STUDY_INDEX_SEQUENCE_GENERATOR_H


#include "07_const_expr_array.h"


struct identity_next;


template <std::size_t n, typename F = identity_next>
struct index_sequence_generator
{
    constexpr static auto make()
    {
        const_expr_array<std::size_t, n> indices{};
        for (std::size_t i = 0; i < n; ++i) {
            // NOTE: C++14 std::array doesn't support
            //          this assignable constexpr [] operator.
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


// some pre-defined number generator functions

struct identity_next
{
    template <typename T>
    constexpr static auto next(T n)
    { return n; }
};

struct next_square_number
{
    template <typename T>
    constexpr static auto next(T n)
    { return n * n; }
};

struct next_even_number
{
    template <typename T>
    constexpr static auto next(T n)
    { return 2 * n; }
};

struct next_odd_number
{
    template <typename T>
    constexpr static auto next(T n)
    { return 2 * n + 1; }
};


#endif  // CPP_TMP_STUDY_INDEX_SEQUENCE_GENERATOR_H
