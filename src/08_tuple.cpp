#include "catch.hpp"

#include <type_traits>
#include <tuple>


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
    auto my_apply_impl(F && f, std::tuple<T...> const& t, std::index_sequence<i...>)
    {
        (void) std::initializer_list<int> { (f(std::get<i>(t)), 0)... };
        return f;
    }

    template <typename F, typename... T>
    auto my_apply(F && f, std::tuple<T...> const& t)
    {
        return my_apply_impl(
                    std::forward<F>(f),
                    t,
                    std::index_sequence_for<T...>{}
            );
    }
}   // un-named namespace


// cf.> std::apply in C++17
TEST_CASE("std::tuple element apply", "[tmp]")
{
    auto t = std::make_tuple(10, "abc", 100.0);

    static_assert(
        std::is_same<
                std::tuple<int, char const *, double>,
                decltype(t)
        >()
    );

    auto result = my_apply(my_func{}, t);
    REQUIRE(result.val_ == 20);
}


template <typename Tuple, std::size_t SizeOfTuple, typename IndexSeq>
struct reverse_tuple_impl;

template <typename... params, std::size_t SizeOfTuple, size_t... i>
struct reverse_tuple_impl<std::tuple<params...>, SizeOfTuple, std::index_sequence<i...>>
{
    using type = std::tuple<
                        std::decay_t<
                                decltype(
                                    std::get<SizeOfTuple - i - 1>(std::tuple<params...>{})
                                )
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

TEST_CASE("reversing std::tuple elements", "[tmp]")
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
