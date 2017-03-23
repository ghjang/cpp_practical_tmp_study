#include "catch.hpp"

#include <type_traits>
#include <string>
#include <tuple>
#include <array>


//==============================================================================
// std::tuple

TEST_CASE("tuple basics", "[tmp]")
{
    // creating std::tuple objects
    auto t = std::tuple<int, std::string, double>{ 10, "abc", 20.0 };
    auto t1 = std::make_tuple(10, "abc", 20.0);

    // getting tuple's size
    static_assert(std::tuple_size<decltype(t)>() == 3);
    static_assert(std::tuple_size<decltype(t1)>() == 3);

    // getting tuple's element type
    static_assert(std::is_same<int, std::tuple_element_t<0, decltype(t)>>());
    static_assert(std::is_same<std::string, std::tuple_element_t<1, decltype(t)>>());
    static_assert(std::is_same<double, std::tuple_element_t<2, decltype(t)>>());

    static_assert(std::is_same<int, std::tuple_element_t<0, decltype(t1)>>());
    static_assert(std::is_same<char const *, std::tuple_element_t<1, decltype(t1)>>());
    static_assert(std::is_same<double, std::tuple_element_t<2, decltype(t1)>>());

    // getting tuple's value
    REQUIRE(std::get<0>(t) == 10);
    REQUIRE(std::get<1>(t) == "abc");
    REQUIRE(std::get<2>(t) == 20.0);

    REQUIRE(std::get<0>(t1) == 10);
    REQUIRE(std::strcmp(std::get<1>(t1), "abc") == 0);
    REQUIRE(std::get<2>(t1) == 20.0);

    REQUIRE(std::get<1>(t1) == "abc");  // This passes also on clang... huh??
}

TEST_CASE("tuple-like types in C++", "[tmp]")
{
    // std::tuple. refer to the above test case.

    // std::pair
    auto p = std::make_pair(10, 20.0);
    static_assert(std::tuple_size<decltype(p)>() == 2);
    static_assert(std::is_same<int, std::tuple_element_t<0, decltype(p)>>());
    static_assert(std::is_same<double, std::tuple_element_t<1, decltype(p)>>());
    REQUIRE(std::get<0>(p) == 10);
    REQUIRE(std::get<1>(p) == 20.0);

    // std::array
    std::array<std::size_t, 5> arr = { 1, 2, 3, 4, 5 };
    static_assert(std::tuple_size<decltype(arr)>() == 5);
    static_assert(std::is_same<std::size_t, std::tuple_element_t<0, decltype(arr)>>());
    static_assert(std::is_same<std::size_t, std::tuple_element_t<4, decltype(arr)>>());
    REQUIRE(std::get<0>(arr) == 1);
    REQUIRE(std::get<4>(arr) == 5);

    // any types supports std::tuple_size, std::get, ...
}


#include <boost/fusion/sequence/intrinsic/size.hpp>
#include <boost/fusion/include/size.hpp>
#include <boost/fusion/sequence/intrinsic/at_c.hpp>
#include <boost/fusion/include/at_c.hpp>
#include <boost/fusion/adapted/struct/adapt_struct.hpp>
#include <boost/fusion/include/adapt_struct.hpp>

namespace boost_fusion_test
{
    struct employee
    {
        std::string name_;
        int age_;
        std::string address_;
    };
}   // namespace boost_fusion_test

BOOST_FUSION_ADAPT_STRUCT(
    boost_fusion_test::employee,
    name_,
    age_,
    address_
)

TEST_CASE("Boost.Fusion struct adaptation", "[tmp]")
{
    namespace fusion = boost::fusion;

    boost_fusion_test::employee emp;

    REQUIRE(fusion::result_of::size<decltype(emp)>::value == 3);

    fusion::at_c<0>(emp) = "name";
    fusion::at_c<1>(emp) = 10;
    fusion::at_c<2>(emp) = "South Korea";

    REQUIRE(emp.name_ == "name");
    REQUIRE(emp.age_ == 10);
    REQUIRE(emp.address_ == "South Korea");
}

#if __clang_major__ >= 4    // if C++17 Structured Bindings is supported,

// refer to:
//      http://boost-experimental.github.io/di/cppnow-2016/#/
//      https://wandbox.org/permlink/b3ecHk9j2u5ueDo4

TEST_CASE("Structured Bindings", "[tmp]")
{

}

#endif

//==============================================================================
// Applying function on std::tuple

namespace
{
    struct my_func
    {
        int val_ = 0;

        void operator () (int i)
        { val_ += i; }

        void operator () (double d)
        { val_ += 10; }

        template <typename T>
        void operator () (T)
        { }
    };


    template <typename F, typename... T, std::size_t... i>
    auto my_foreach_impl(F && f, std::tuple<T...> const& t, std::index_sequence<i...>)
    {
        (void) std::initializer_list<int> { (f(std::get<i>(t)), 0)... };
        return f;
    }

    template <typename F, typename... T>
    auto my_foreach(F && f, std::tuple<T...> const& t)
    {
        return my_foreach_impl(
                    std::forward<F>(f),
                    t,
                    std::index_sequence_for<T...>{}
                );
    }
}   // un-named namespace


TEST_CASE("foreach on std::tuple element", "[tmp]")
{
    auto t = std::make_tuple(10, "abc", 100.0);

    static_assert(
        std::is_same<
                std::tuple<int, char const *, double>,
                decltype(t)
        >()
    );

    auto result = my_foreach(my_func{}, t);
    REQUIRE(result.val_ == 20);
}

#if __clang_major__ >= 4    // if C++17 std::apply is available,
struct my_func_for_apply
{
    auto operator () (int i, char const *, double d)
    {
        return i + 20;
    }

    auto operator () (int i, char const *)
    {
        return i + 20;
    }

    template <typename... T>
    auto operator () (T... t)
    {
        return (t + ...);   // C++17 fold expression. fold-right.
                            // return the sum.
    }
};

// std::apply works on any tuple-like objects
//  that supports std::get and std::tuple_size.
//  ex.> std::tuple, std::pair, std::array and etc.
TEST_CASE("std::apply with std::tuple", "[tmp]")
{
    auto t = std::make_tuple(10, "abc", 100.0);

    static_assert(std::tuple_size_v<decltype(t)> == 3);
    REQUIRE(std::get<0>(t) == 10);

    REQUIRE(std::apply(my_func_for_apply{}, t) == 30);
}

TEST_CASE("std::apply with std::pair", "[tmp]")
{
    auto p = std::make_pair(10, "abc");

    static_assert(std::tuple_size_v<decltype(p)> == 2);
    REQUIRE(std::get<0>(p) == 10);

    REQUIRE(std::apply(my_func_for_apply{}, p) == 30);
}

TEST_CASE("std::apply with std::array", "[tmp]")
{
    std::array<std::size_t, 5> arr = { 1, 2, 3, 4, 5 };

    static_assert(std::tuple_size_v<decltype(arr)> == 5);
    REQUIRE(std::get<0>(arr) == 1);

    REQUIRE(std::apply(my_func_for_apply{}, arr) == 15);
}
#endif


//==============================================================================
// reversing std::tuple's element types

template <typename Tuple, std::size_t SizeOfTuple, typename IndexSeq>
struct reverse_tuple_impl;

template <typename... params, std::size_t SizeOfTuple, size_t... i>
struct reverse_tuple_impl<std::tuple<params...>, SizeOfTuple, std::index_sequence<i...>>
{
    using type = std::tuple<
                        std::tuple_element_t<
                                SizeOfTuple - i - 1,
                                std::tuple<params...>
                        >...
                 >;
};

template <typename Tuple>
struct reverse_tuple;

template <typename... params>
struct reverse_tuple<std::tuple<params...>>
            : reverse_tuple_impl<
                    std::tuple<params...>,
                    sizeof...(params),
                    std::index_sequence_for<params...>
              >
{ };

TEST_CASE("reversing std::tuple's element types", "[tmp]")
{
    using tuple_t = std::tuple<int, std::string, double>;
    using reversed_t = std::tuple<double, std::string, int>;
    static_assert(
        std::is_same<
                reversed_t,
                reverse_tuple<tuple_t>::type
        >()
    );
}
