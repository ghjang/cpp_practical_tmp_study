#include "catch.hpp"

#include "type.h"


//==============================================================================
// runtime function f
template <typename Arg>
Arg f(Arg a)
{
    return a;           // returns a value
}

// compile-time function F
template <typename Arg>
struct F
{
    using type = Arg;   // returns a type
};

// using type alias for convenience
template <typename Arg>
using f_t = typename F<Arg>::type;

TEST_CASE("metafunction basic concept", "[tmp]")
{
    auto a = f(100);

    F<int>::type b = 200;

    f_t<int> c = 300;
}

//==============================================================================
template <typename T>
struct add_pointer
{
    using type = T *;
};

template <typename T>
struct add_pointer_fwd
            : type_is<T *>  // metafunction forwarding
{ };

template <typename T>
using add_pointer_fwd_t = typename add_pointer_fwd<T>::type;

TEST_CASE("metafunction forwarding and type_is utility metafunction", "[tmp]")
{
    // add_pointer metafunction call
    using my_ptr = add_pointer<int>::type;
    int i = 100;
    my_ptr ptrI = &i;

    using my_ptr_ptr = add_pointer<int *>::type;
    my_ptr_ptr ptrPtrI = &ptrI;

    // add_pointer_fwd metafunction call
    using my_another_ptr = add_pointer_fwd<double>::type;
    double d = 100.;
    my_another_ptr ptrD = &d;

    // add_pointer_fwd_t type alias call
    using my_another_ptr1 = add_pointer_fwd_t<double>;
    my_another_ptr1 ptrD1 = &d;
}
