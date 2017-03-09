#include "catch.hpp"

#include <type_traits>
#include <string>

//==============================================================================
TEST_CASE("std::is_same", "[tmp]")
{

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
    // OK
    my_integer_container<int> c;

    // NOT OK
    //my_integer_container<double> c1;
    //my_integer_container<std::string> c2;
}

//==============================================================================
TEST_CASE("std::conditional", "[tmp]")
{

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

//==============================================================================
TEST_CASE("std::integral_constant", "[tmp]")
{

}
