#include "catch.hpp"

#include <type_traits>
#include <tuple>

#include "metafunction_def.h"


TEST_CASE("std::common_type", "[tmp]")
{
    static_assert(std::is_same<long long, std::common_type_t<int, long long>>());
    static_assert(std::is_same<double, std::common_type_t<int, double>>());

    // This is an expected compile error.
    // 'int' and 'char const *' has no common type
    //  so the std::common_type produces no 'type' member.
    //static_assert(std::is_same<void, std::common_type_t<int, char const *>>());
}


template <typename... T>
std::common_type_t<T...>
get_common_type(TL<T...>, int);

template <typename... T>
void
get_common_type(TL<T...>, ...);


template <typename... T, std::size_t... i, std::size_t... j>
auto make_pair_array_impl(std::tuple<T...> && t,        // the arguments
                          std::index_sequence<i...>,    // even indices of the arguments
                          std::index_sequence<j...>)    // odd indices of the arguments
{
    static_assert(sizeof...(i) == sizeof...(j));
    static_assert(sizeof...(T) == sizeof...(i) * 2);

    using first_t = decltype(
                        get_common_type(
                            TL<
                                std::tuple_element_t<
                                        i,
                                        std::tuple<T...>
                                >...
                            >{},
                            0
                        )
                    );
    static_assert(
        !std::is_void<first_t>(), "there is no common type for first member of pair."
    );
    static_assert(
        std::is_integral<first_t>(), "first member type of pair should be integral."
    );

    using second_t = decltype(
                        get_common_type(
                            TL<
                                std::tuple_element_t<
                                        j,
                                        std::tuple<T...>
                                >...
                            >{},
                            0
                        )
                     );
    static_assert(
        !std::is_void<second_t>(), "there is no common type for second member of pair."
    );

    using element_t = std::conditional_t<
                            std::is_same<second_t, char const *>::value,
                            std::pair<first_t, std::string>,
                            std::pair<first_t, second_t>
                      >;

    return std::array<element_t, sizeof...(i)>{
                std::make_tuple(std::get<i>(t), std::get<j>(t))...
           };   
}


// 
template <typename... T>
auto make_pair_array(T &&... args)
{
    static_assert(sizeof...(args) > 0, "there is no arguments.");
    static_assert(sizeof...(args) % 2 == 0, "number of args should be even.");

    using even_index_seq_t = make_even_index_sequence_t<sizeof...(args) / 2>;
    using odd_index_seq_t = make_odd_index_sequence_t<sizeof...(args) / 2>;

    return make_pair_array_impl(
                std::forward_as_tuple(std::forward<T>(args)...),
                even_index_seq_t{},
                odd_index_seq_t{}
           );
}

TEST_CASE("make_pair_array function template example", "[tmp]")
{
    auto pairArr = make_pair_array(
                        1, "abc",
                        2, "defg",
                        3, "hijkl"
                   );
    REQUIRE(std::size(pairArr) == 3);

    using pair_elem_t = std::decay_t<decltype(pairArr[0])>;
    static_assert(std::is_same<int, pair_elem_t::first_type>());
    static_assert(std::is_same<std::string, pair_elem_t::second_type>());

    REQUIRE(pairArr[0].first == 1);
    REQUIRE(pairArr[0].second == "abc");
    REQUIRE(pairArr[1].first == 2);
    REQUIRE(pairArr[1].second == "defg");
    REQUIRE(pairArr[2].first == 3);
    REQUIRE(pairArr[2].second == "hijkl");

    for (auto & pair : pairArr) {
        //pair.first;
        //pair.second;
        // ...
    }

    #if __clang_major__ >= 4    // if C++17 Structured Bindings is available,
        for (auto & [key, value] : pairArr) {
            // ...
        }
    #endif

    // NOTE: This is an expected compile error.
    //          Number of arguments is odd.
    /*
    auto pairArr1 = make_pair_array(
                        1, "abc",
                        2, "def",
                        3, "ghi",
                        4
                    );
    */

    // NOTE: This is an expected compile error.
    //          No arguments.
    //auto pairArr2 = make_pair_array();

    // NOTE: This is an expected compile error.
    //          The argument 3.0 is not integral value.
    /*
    auto pairArr3 = make_pair_array(
                        1, "abc",
                        2, "defg",
                        3.0, "hijkl"
                    );
    */

    // NOTE: This is an expected compile error.
    //          The argument 1024 and string literals don't have a common type.
    /*
    auto pairArr4 = make_pair_array(
                        1, "abc",
                        2, "defg",
                        3, 1024
                    );
    */

    struct S
    { 
        int i_;
    };

    auto pairArr5 = make_pair_array(
                        1, S{ 1 },
                        2, S{ 2 },
                        3, S{ 3 }
                    );
}
