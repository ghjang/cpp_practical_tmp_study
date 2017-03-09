#include "catch.hpp"

// global
static_assert(true);

namespace
{
    // in a namespace
    static_assert(true);

    struct s
    {
        // in a class
        static_assert(true);

        void f()
        {
            // in a member function
            static_assert(true);
        }
    };

    template <typename T>
    struct my_struct
    {
        // in a class template
        // You can make some assertions on the type T.
        static_assert(true);

        template <typename U>
        void f()
        {
            // in a member function template
            // You can make some assertions on the type T and U.
            static_assert(true);
        }
    };
} // un-named namespace

TEST_CASE("static_assert", "[tmp]")
{
    // in a local scope
    static_assert(true, "this is true.");

    // added in C++17
    static_assert(true);

    //static_assert(false, "this is false.");
}
