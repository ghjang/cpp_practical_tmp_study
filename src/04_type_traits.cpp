#include "catch.hpp"

#include <type_traits>
#include <string>

//==============================================================================
TEST_CASE("std::is_same", "[tmp]")
{
    static_assert(std::is_same<int, int>::value);
    static_assert(std::is_same<int, int>());    // C++14 operator () ()

    static_assert(!std::is_same<int, double>());
}

//==============================================================================
template <typename T>
struct my_integer_container
{
    static_assert(std::is_integral<T>(), "T should be integral type.");

    T data_[100];
};

TEST_CASE("std::is_integral and static_assert", "[tmp]")
{
    static_assert(std::is_integral<bool>());
    static_assert(std::is_integral<int>());
    static_assert(!std::is_integral<float>());
    static_assert(!std::is_integral<double>());

    // OK
    my_integer_container<int> c;

    // NOT OK
    //my_integer_container<double> c1;
    //my_integer_container<std::string> c2;
}

//==============================================================================
template <typename T, int n>
constexpr auto my_int_const_func(T t, std::integral_constant<int, n>)
{
    return t * t;
}

template <typename T>
constexpr auto my_int_const_func(T t, std::integral_constant<int, 3>)
{
    return t * t * t;
}

TEST_CASE("std::integral_constant", "[tmp]")
{
    static_assert(
        my_int_const_func(2, std::integral_constant<int, 2>{}) == 4     // 2^2
    );
    static_assert(
        my_int_const_func(3, std::integral_constant<int, 3>{}) == 27    // 3^3
    );
    static_assert(
        my_int_const_func(4, std::integral_constant<int, 4>{}) == 16    // 4^2
    );
}

template <typename T>
constexpr auto my_bool_tag_dispatch_func_impl(T t, std::true_type)
{
    return t;
}

template <typename T>
constexpr auto my_bool_tag_dispatch_func_impl(T t, std::false_type)
{
    return t * 2;
}

template <typename T>
constexpr auto my_bool_tag_dispatch_func(T t)
{
    return my_bool_tag_dispatch_func_impl(t, std::is_same<T, int>());

    // if C++17 'if constexpr' is available, following is possible:
    /*
        if constexpr (std::is_same<T, int>()) {
            return t;
        } else {
            return t * 2;
        }
    */
}

TEST_CASE("std::true_type, std::false_type", "[tmp]")
{
    static_assert(std::is_same<std::true_type, std::integral_constant<bool, true>>());
    static_assert(std::is_same<std::false_type, std::integral_constant<bool, false>>());

    static_assert(my_bool_tag_dispatch_func(100) == 100);
    static_assert(my_bool_tag_dispatch_func(100L) == 200L);
}

//==============================================================================
struct base_a
{
    template <typename T>
    auto calc(T t)
    { return t; }
};

struct base_b
{
    template <typename T>
    auto calc(T t)
    { return t * t; }
};

template <typename Base>
struct derived : Base
{
    template <typename T>
    auto do_it(T t)
    { return Base::calc(t); }
};

template <typename T>
struct select_base
            : std::conditional<
                    std::is_floating_point<T>::value,
                    base_a,
                    base_b
              >
{ };

template <typename T>
auto make_derived()
{
    return derived<
                typename select_base<T>::type
           >{};
}

TEST_CASE("std::conditional", "[tmp]")
{
    using my_int_t = int;
    using my_num_t = int;

    struct S { };
    struct T { };

    using my_data_t = std::conditional_t<
                            std::is_same<my_int_t, my_num_t>::value,
                            S,
                            T
                      >;
    static_assert(std::is_same<S, my_data_t>());

    auto derivedObj = make_derived<int>();
    REQUIRE(derivedObj.do_it(10) == 100);
    
    auto derivedObj1 = make_derived<double>();
    REQUIRE(derivedObj1.do_it(10) == 10);
}

//==============================================================================
TEST_CASE("std::decay", "[tmp]")
{

}

//==============================================================================
TEST_CASE("std::enable_if", "[tmp]")
{

}

//==============================================================================
TEST_CASE("std::result_of", "[tmp]")
{
    
}
