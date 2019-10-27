#include "catch.hpp"

#include <type_traits>
#include <tuple>


//==============================================================================
// NOTE: template parameter pack expansions are not allowed in the structured
//       bindings' identifier-list context.
//
//       refer to https://en.cppreference.com/w/cpp/language/parameter_pack

template <typename... T>
void fvp(T &... t)
{
    struct S
    {
        int i_ = 10;
        std::string s_ = "abc";
        float f_ = 10.5f;
    };

    // NOTE: This is NOT allowed.
    //auto [t...] = S{};

    // FYI, there seemed to be a proposal like the following:
    //  http://open-std.org/JTC1/SC22/WG21/docs/papers/2018/p1061r0.html?fbclid=IwAR3CL-9DVXdYc8rYhVrwRKDQJDzRcI5u8r07eWwlaF__rENLn5-G_LqAGCg
}

TEST_CASE("structured bindings with variadic template parameters", "[tmp]")
{
    int i = 0;
    std::string s;
    float f = 0;

    fvp(i, s, f);
}
