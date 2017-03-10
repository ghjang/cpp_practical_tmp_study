# About C++ Template Metaprogramming

## Basics Review

1. C++ Language Features Related to C++ TMP
    1. static_assert: C++11
    2. sizeof
    3. decltype: C++11
    4. template
        1. type template parameter
        2. non-type template parameter
        3. full specialization
        4. partial specialization
        5. template template parameter
        6. variadic template: C++11
    5. using type alias, using type alias template: C++11
    6. variable template: C++14
    7. constexpr: C++11
    8. constexpr if: C++17
    9. user-defined literals: C++11
    10. fold expression: C++17
    11. structured binding: C++17

2. C++ Library Features Related to C++ TMP
    1. type_traits: C++11
        1. std::integral_constant
        2. std::decay
        3. std::enable_if
        4. std::conditional
        5. std::void_t: C++17
        6. etc.
    2. std::declval: C++11
    3. std::integer_sequence: C++14
    4. std::initializer_list: C++11
    5. std::tuple: C++11
    6. std::variant: C++17
    7. std::optional: C++17

## C++ Template Metaprogramming Concept

1. Meta-function
    1. numeric calculation
    2. type calculation
2. Meta-function Forwarding
3. Meta-function Class
4. SFINAE(Substitution Failure Is Not An Error)

## Some Useful C++ Template Metaprogramming Idiom

1. Overload Resolution Management
2. Integer Sequence Manipulation

## Implementing Some Primitive Metafunctions

1. identity
2. type_is
3. type_list, TL
4. length
5. head
6. tail
7. last
8. init
9. map
10. filter
11. foldl, foldr
12. compose
13. bind_first, bind_last
14. reverse
15. find
16. unique
17. ...

## Examples

1. Member Test
2. Integer Sequence Generation
3. make_variant_vector
4. tuple_cat
5. Aligned Tuple
6. ...

---

## References

+ [What Does Haskell Have to Do with C++?](https://bartoszmilewski.com/2009/10/21/what-does-haskell-have-to-do-with-c/): Bartosz Milewski's blog post
+ [Tiny Metaprogramming Library](http://ericniebler.com/2014/11/13/tiny-metaprogramming-library/): Eric Niebler's blog post
+ [Simple C++11 metaprogramming](): Peter Dimov's blog post
+ [Practical C++ Metaprogramming](http://www.oreilly.com/programming/free/practical-c-plus-plus-metaprogramming.csp): 54-page PDF file report about C++ TMP
+ [CppCon 2015: Louis Dionne “C++ Metaprogramming: A Paradigm Shift"](https://www.youtube.com/watch?v=cg1wOINjV9U)
+ [C++ Now 2016: Louis Dionne: Metaprogramming for dummies](https://www.youtube.com/watch?v=a1doqFAumCk&index=9&list=PL_AKIMJc4roU0F3w20Ac77YeOFyvFmaJD)
+ [C++ Now 2016: Louis Dionne: Metaprogramming for the brave](https://www.youtube.com/watch?v=UXwWXHrvTug&index=16&list=PL_AKIMJc4roU0F3w20Ac77YeOFyvFmaJD)
+ Books
    - [C++ Templates: The Complete Guide](https://www.amazon.com/Templates-Complete-Guide-David-Vandevoorde/dp/0201734842/ref=sr_1_1?ie=UTF8&qid=1489142301&sr=8-1&keywords=C%2B%2B+templates)
    - [C++ Template Metaprogramming: Concepts, Tools, and Techniques from Boost and Beyond](https://www.amazon.com/Template-Metaprogramming-Concepts-Techniques-Beyond/dp/0321227255/ref=sr_1_1?ie=UTF8&qid=1489142222&sr=8-1&keywords=C%2B%2B+template+metaprogramming)
    - [Modern C++ Design: Generic Programming and Design Patterns Applied](https://www.amazon.com/Modern-Design-Generic-Programming-Patterns/dp/0201704315/ref=sr_1_1?ie=UTF8&qid=1489142257&sr=8-1&keywords=modern+c%2B%2B+design)
+ C++ Template Metaprogramming Libraries
    - [Boost.MPL](http://www.boost.org/doc/libs/1_63_0/libs/mpl/doc/index.html)
    - [Boost.Fusion](http://www.boost.org/doc/libs/1_63_0/libs/fusion/doc/html/index.html)
    - [Boost.Hana](http://www.boost.org/doc/libs/1_63_0/libs/hana/doc/html/index.html)
    - [Brigand](https://github.com/edouarda/brigand/wiki)
