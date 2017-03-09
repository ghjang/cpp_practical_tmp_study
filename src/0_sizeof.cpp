#include "catch.hpp"

#include <type_traits>
#include <tuple>

#include "0_sizeof.h"


//==============================================================================
struct S
{
    int i_;
    float f_;
};

// NOTE: functions don't have body.
int f();
S g();

template <typename... T>
constexpr auto num_of_args(T... t)
{
    // used the type name T
    return sizeof...(T);
}

template <typename... T>
constexpr auto num_of_args_1(T... t)
{
    // used local variable t.
    return sizeof...(t);
}

template <typename... T>
struct my_container;

TEST_CASE("sizeof, sizeof...", "[tmp]")
{
    static_assert(sizeof(int) == sizeof(f()));
    static_assert(sizeof(S) == sizeof(g()));

    int i;
    S objS; 
    static_assert(sizeof(i) == sizeof(f()));
    static_assert(sizeof(objS) == sizeof(g()));

    static_assert(num_of_args(1, 1.0, 'a') == 3);
    static_assert(num_of_args_1(1, 1.0, 'a') == 3);

    // std::tuple
    static_assert(
        length<
            std::tuple<char, int, float, double>
        >::value == 4
    );

    // local user-defined struct my_container
    static_assert(
        length<
            my_container<char, int, float, double>
        >::value == 4
    );
}

TEST_CASE("decltype", "[tmp]")
{
    static_assert(
        std::is_same<int, decltype(f())>()
    );
    static_assert(
        std::is_same<S, decltype(g())>()
    );

    int i;
    S objS;
    static_assert(
        std::is_same<int, decltype(i)>()
    );
    static_assert(
        std::is_same<S, decltype(objS)>()
    );
}
