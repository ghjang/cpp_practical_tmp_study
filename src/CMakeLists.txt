cmake_minimum_required(VERSION 3.0.0)
project(cpp_tmp_test VERSION 0.0.0)

include(CTest)
enable_testing()

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

set(BOOST_ROOT "../../boost_1_63_0")
find_package(Boost 1.63.0)
if(Boost_FOUND)
    include_directories(${BOOST_ROOT})
else()
    message(FATAL_ERROR "Could not find boost!")
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1z")

set(SOURCE_FILES
        main.cpp
        00_static_assert.cpp
        01_using_type_alias.cpp
        03_metafunction.cpp
        05_sizeof_decltype.cpp
        06_type_traits.cpp
        09_integer_sequence.cpp
        10_tuple.cpp
        11_make_pair_array.cpp
        12_n-depth_loop.cpp
        13_metafunction_class.cpp
        14_structured_bindings.cpp)

add_executable(cpp_tmp_test ${SOURCE_FILES})
