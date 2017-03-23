#ifndef CPP_TMP_STUDY_CONST_EXPR_ARRAY_H
#define CPP_TMP_STUDY_CONST_EXPR_ARRAY_H


// use std::array instead in C++17
template <typename T, std::size_t N>
struct const_expr_array
{
    constexpr std::size_t size() const
    { return N; }

    constexpr T const& operator [] (std::size_t i) const
    { return array_[i]; }

    // NOTE: C++14 std::array doesn't declare this overloaded operator [] as constexpr.
    constexpr T & operator [] (std::size_t i)
    { return array_[i]; }

    T array_[N]{};
};


#endif  // CPP_TMP_STUDY_CONST_EXPR_ARRAY_H
