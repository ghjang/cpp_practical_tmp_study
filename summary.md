# 2017/03/10

* `static_assert`
* using type alias
* metafunction concept & examples
    + numeric calculation metafunction
        - `my_func<...>::value`, returns 'value'.
    + type calculation metafunction
        - `my_func<...>::type`, returns 'type'.
    + metafunction forwarding
        - `type_is` metafunction
* C++14 variable template
* `length` metafunction

# 2017/03/17

* `sizeof`
* `decltype`
* standard metafunctions & examples
    + `std::is_same`
    + `std::integral_constant`
    + `std::conditional`
    + `std::decay`
    + `std::enable_if`
    + `std::declval`
    + `std::result_of`
        - cf.> C++17 `std::is_invocable`, `std::invoke_result`, `std::invoke`
* SFINAE
* tag dispatch pattern
* C++17 constexpr if

# 2017/03/24

* C++14 `std::integer_sequence` & integer sequence generation examples
    + `std::index_sequence`
    + `std::make_integer_sequence`
    + `std::make_index_sequence`
    + `std::index_sequence_for`
* `std::array`
    + cf.> `const_expr_array`
* `std::tuple` & examples
    + `std::make_tuple`
    + `std::tuple_size`
    + `std::tuple_element`
    + `std::get`
    + tuple-like types in C++
        - `std::pair`
        - `std::array`
        - any types support `std::tuple_size`, `std::get`, ...
        - cf.> Boost.Fusion struct adaptation, C++17 Structured Bindings
    + C++17 `std::apply`
