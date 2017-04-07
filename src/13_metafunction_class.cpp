#include "catch.hpp"

#include <type_traits>
#include <tuple>

//==============================================================================
// metafunction class

// a metafunction for type calculation
template <typename T>
struct metafunction
{
    using type = T;     // do some calculation on T.
};

// a metafunction class containing a metafunction
//  of which name is 'lambda', 'apply' or something like that.
struct metafunction_class
{
    template <typename T>
    struct lambda
    {
        using type = T;
    };
};

TEST_CASE("metafunction class", "[tmp]")
{
    // normal metafunction call
    static_assert(std::is_same_v<int, metafunction<int>::type>);

    // metafunction class call
    static_assert(std::is_same_v<int, metafunction_class::lambda<int>::type>);
}

//==============================================================================
// metafunction class semantics: generalizing template to type

// higher_order_metafunction_0 expects a normal template template parameter F.
template <template <typename...> class F, typename T>
struct higher_order_metafunction_0
{
    using type = typename F<T>::type;
};

// higher_order_metafunction_1 expects a type parameter F which is a metafunction class.
template <typename F, typename T>
struct higher_order_metafunction_1
{
    using type = typename F::template lambda<T>::type;
};

// higher_order_metafunction_2 expects a type parameter F which is a metafunction class.
template <typename F, typename T>
struct higher_order_metafunction_2
{
    using type = std::pair<F, T>;
};


// Forwarding template template parameter to another metafunction function
//  which also accepts template template parameter is OK.
template <template <typename...> class F, typename T>
struct my_metafunc_0
{
    using type = typename higher_order_metafunction_0<F, T>::type;
};

// But, this is a compile error.
// A template is NOT a type. You CAN'T pass it as a type parameter.
/*
template <template <typename...> class F, typename T>
struct my_metafunc_1
{
    using type = typename higher_order_metafunction_2<F, T>::type;
};
*/

TEST_CASE("metafunction class for generalizing template to type", "[tmp]")
{
    // passing normal metafunction
    static_assert(
        std::is_same_v<
                int,
                higher_order_metafunction_0<metafunction, int>::type
        >
    );

    // passing metafunction class
    static_assert(
        std::is_same_v<
                int,
                higher_order_metafunction_1<metafunction_class, int>::type
        >
    );

    // passing normal metafunction
    static_assert(
        std::is_same_v<
                int,
                my_metafunc_0<metafunction, int>::type
        >
    );
}

//==============================================================================
// transforming metafunction to metafunction class

template <template <typename...> class f>
struct quote
{
    template <typename... xs>
    struct lambda : f<xs...>
    { };
};

TEST_CASE("metafunction to metafunction class", "[tmp]")
{
    // note that 'metafunction' is a normal metafunction here.
    static_assert(
        std::is_same_v<
                int,
                quote<metafunction>::lambda<int>::type
        >
    );

    // note that 'higher_order_metafunction_1' expects a metafunction class
    //  at the first template parameter here.
    static_assert(
        std::is_same_v<
                int,
                higher_order_metafunction_1<
                    quote<metafunction>,
                    int
                >::type
        >
    );

    static_assert(
        std::is_same_v<
                std::true_type,
                higher_order_metafunction_1<
                    quote<std::is_integral>,
                    int
                >::type
        >
    );

    static_assert(
        higher_order_metafunction_1<
            quote<std::is_integral>,
            int
        >::type::value
    );
}

//==============================================================================
// making calling metafunction class easier

template <typename f, typename... xs>
struct invoke : f::template lambda<xs...>
{ };

template <typename f, typename... xs>
using invoke_t = typename invoke<f, xs...>::type;

// 'F' is required to be a metafunction class
template <typename F, typename T>
struct my_metafunc_2
        : invoke<F, T>
{ };

TEST_CASE("invoking metafunction class with invoke", "[tmp]")
{
    static_assert(
        std::is_same_v<
            std::true_type,
            invoke_t<quote<std::is_integral>, int>
        >
    );

    static_assert(
        std::is_same_v<
            std::true_type,
            my_metafunc_2<quote<std::is_integral>, int>::type
        >
    );
}

//==============================================================================
// comparing with function template argument

template <typename F, typename T>
auto my_func(F f, T arg)
{ return f(arg); }

int square(int i)
{ return i * i; }

struct triple
{
    int operator () (int i)
    { return 3 * i; }
};

TEST_CASE("comparing with function template argument", "[tmp]")
{
    // You can pass a normal function.
    REQUIRE(my_func(&square, 2) == 4);

    // You can also pass a function-like object.
    REQUIRE(my_func(triple{}, 2) == 6);

    // You can also pass a lambda object.
    REQUIRE(my_func(
                [](int i){ return i * i * i; },
                2
            ) == 8);

    // You can pass any function-like entities to the function template.
    // This means that the function template argument is fully generalized.

    // metafunction class is for providing us with an uniform way of
    //  passing an normal metafunction to a higher-order metafunction.

    // cf.> You can also think that std::integral_constant is an generalization of
    //          integral values to types. This makes us possible to pass
    //          a typed-integer value to a metafunction that expects a type parameter.
}
