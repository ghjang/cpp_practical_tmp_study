#include "catch.hpp"

#include <tuple>


TEST_CASE("type alias", "[tmp]")
{
    // typedef
    typedef unsigned long long my_int_t;
    my_int_t i = 100;

    typedef int(* F)(char);
    F f = nullptr;

    // using
    using my_int_u = unsigned long long;
    my_int_u j = 200;

    using U = int(*)(char);
    U u = nullptr;
}


template <typename T>
using my_int = T;

template <typename... T>
using my_data = std::tuple<T...>;

template <template <typename...> class T>
struct my_template
{
    T<int, double> t_;
};

TEST_CASE("using type alias template", "[tmp]")
{
    // using type alias can be a template.
    my_int<long> k = 300;
    auto data = my_data<int, double>{ 400, 10.0 };

    // passing using template as template template parameter.
    my_template<my_data> t;
}
