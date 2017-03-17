#include "catch.hpp"

#include <type_traits>
#include <tuple>

#include "04_length.h"


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

//==============================================================================
TEST_CASE("std::declval function template", "[tmp]")
{
    struct S
    {
        int f() const;
    };

    struct T
    {
        T();
        long f() const;
    };

    struct U
    {
        U(int i);
        double f() const;
    };

    static_assert(std::is_default_constructible<S>());
    static_assert(std::is_default_constructible<T>());
    static_assert(!std::is_default_constructible<U>());

    static_assert(std::is_same<int, decltype(S{}.f())>());
    static_assert(std::is_same<long, decltype(T{}.f())>());

    //std::is_same<double, decltype(U{}.f())>();    // compile error

    static_assert(
        std::is_same<
                double,
                decltype(std::declval<U>().f())
        >()
    );

    static_assert(
        std::is_same<
                U &&,
                decltype(std::declval<U>())
        >()
    );
}

//==============================================================================
struct struct_0
{
    using type = int;
};

struct struct_1
{ };


template <typename T, typename = typename T::type>
constexpr int type_sfinae_test(T t) { return 0; }

constexpr int type_sfinae_test(int i) { return 1; }

TEST_CASE("type SFINAE", "[tmp]")
{
    static_assert(type_sfinae_test(struct_0{}) == 0);
    
    // compile error
    //type_sfinae_test(struct_1{});

    static_assert(type_sfinae_test(0) == 1);
}


struct X
{
    int x_;
};

X operator + (X const& lhs, X const& rhs)
{
    return { lhs.x_ + rhs.x_ };
}

struct Y
{
    int y_;
};

template <typename T>
auto expr_sfinae_test(T t0, T t1) -> decltype(t0 + t1)
{
    return t0 + t1;
}

auto expr_sfinae_test(Y t0, Y t1)
{
    return t0;
}

TEST_CASE("expression SFINAE", "[tmp]")
{
    REQUIRE(expr_sfinae_test(X{ 1 }, X{ 2 }).x_ == 3);
    REQUIRE(expr_sfinae_test(Y{ 1 }, Y{ 2 }).y_ == 1);
}


template <typename T>
struct has_type_member_with_sizeof
{
    using yes = char [1];
    using no = char [2];

    template <typename U>
    static yes & test(typename U::type *);

    template <typename U>
    static no & test(...);

    constexpr static auto value = sizeof(test<T>(nullptr)) == sizeof(yes);
};

// refer to http://en.cppreference.com/w/cpp/language/sfinae
TEST_CASE("member existence check with sizeof", "[tmp]")
{
    static_assert(has_type_member_with_sizeof<struct_0>::value);
    static_assert(!has_type_member_with_sizeof<struct_1>::value);
}


template <typename T>
struct has_type_member_with_decltype
{
    template <typename U, typename = typename U::type>
    static std::true_type test(int);

    template <typename U>
    static std::false_type test(...);

    constexpr static auto value = decltype(test<T>(0)){};
};

TEST_CASE("member existence check with decltype", "[tmp]")
{
    static_assert(has_type_member_with_decltype<struct_0>::value);
    static_assert(!has_type_member_with_decltype<struct_1>::value);
}


// use C++17 std::void_t if available.
template <typename...>
using void_t = void;

template <typename T, typename = void>
struct has_type_member_with_void_t
            : std::false_type
{ };

template <typename T>
struct has_type_member_with_void_t<T, void_t<typename T::type>>
            : std::true_type
{ };

TEST_CASE("member existence check with void_t", "[tmp]")
{
    static_assert(has_type_member_with_void_t<struct_0>::value);
    static_assert(!has_type_member_with_void_t<struct_1>::value);
}
