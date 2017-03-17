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
constexpr auto int_const_test(T t, std::integral_constant<int, n>)
{
    return t * t;
}

template <typename T>
constexpr auto int_const_test(T t, std::integral_constant<int, 3>)
{
    return t * t * t;
}

TEST_CASE("std::integral_constant", "[tmp]")
{
    static_assert(
        int_const_test(2, std::integral_constant<int, 2>{}) == 4     // 2^2
    );
    static_assert(
        int_const_test(3, std::integral_constant<int, 3>{}) == 27    // 3^3
    );
    static_assert(
        int_const_test(4, std::integral_constant<int, 4>{}) == 16    // 4^2
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
// decay_test has no body here.
template <typename T>
T decay_test(T t);

int my_func();

TEST_CASE("std::decay", "[tmp]")
{
    int i = 100;
    int & ri = i;
    int const j = 200;
    int const volatile k = 300;

    static_assert(std::is_same<int, decltype(decay_test(i))>());
    static_assert(std::is_same<int, decltype(decay_test(ri))>());
    static_assert(std::is_same<int, decltype(decay_test(j))>());
    static_assert(std::is_same<int, decltype(decay_test(k))>());

    static_assert(std::is_same<int, std::decay_t<decltype(i)>>());
    static_assert(std::is_same<int, std::decay_t<decltype(ri)>>());
    static_assert(std::is_same<int, std::decay_t<decltype(j)>>());
    static_assert(std::is_same<int, std::decay_t<decltype(k)>>());

    // function-to-pointer decay
    static_assert(
        std::is_same<
                int (),         // function type
                decltype(my_func)
        >()
    );
    static_assert(
        std::is_same<
                int (*)(),      // function pointer type.
                decltype(decay_test(my_func))   // template type deduction
        >()
    );
    static_assert(
        std::is_same<
                int (*)(),      // function pointer type
                std::decay_t<decltype(my_func)>
        >()
    );

    // array-to-pointer decay
    int a[10];
    static_assert(
        std::is_same<
                int [10],       // array type
                decltype(a)
        >()
    );
    static_assert(
        std::is_same<
                int *,          // pointer (to array) type
                decltype(decay_test(a))
        >()
    );
    static_assert(
        std::is_same<
                int *,          // pointer (to array) type
                std::decay_t<decltype(a)>
        >()
    );
}

//==============================================================================
template
<
    typename T,
    typename = std::enable_if_t<std::is_integral<T>::value>
>
constexpr auto do_calc(T t)
{
    return t * t;
}

template
<
    typename T,
    typename = void,    // NOTE: to fix compile error
    typename = std::enable_if_t<std::is_floating_point<T>::value>
>
constexpr auto do_calc(T t)
{
    return t;
}


template <typename T, typename = void>
struct calculator;

template <typename T>
struct calculator<T, std::enable_if_t<std::is_integral<T>::value>>
{
    static auto do_calc(T t)
    {
        return t * t;
    }
};

template <typename T>
struct calculator<T, std::enable_if_t<std::is_floating_point<T>::value>>
{
    static auto do_calc(T t)
    {
        return t;
    }
};

TEST_CASE("std::enable_if", "[tmp]")
{
    static_assert(std::is_same<void, std::enable_if<true>::type>());
    
    // compile error. 'no type member'.
    //std::enable_if<false>::type{};

    static_assert(std::is_same<int, std::enable_if<true, int>::type>());

    static_assert(do_calc(10) == 100);
    static_assert(do_calc(10.0) == 10);

    REQUIRE(calculator<int>::do_calc(10) == 100);
    REQUIRE(calculator<double>::do_calc(10) == 10);
}

//==============================================================================
struct callable_obj
{
    auto operator () (int i, int j) const
    {
        return 1.0;     // double
    }

    auto operator () (double i, double j) const
    {
        return 1;       // int
    }
};

TEST_CASE("std::result_of, std::invoke_result", "[tmp]")
{
    // NOTE: std::result_of is deprecated in C++17.
    //          Use std::invoke_result instead in C++17.

    static_assert(std::is_same<double, std::result_of_t<callable_obj(int, int)>>());
    static_assert(std::is_same<int, std::result_of_t<callable_obj(double, double)>>());
}
