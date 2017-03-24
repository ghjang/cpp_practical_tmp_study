#ifndef CPP_TMP_STUDY_TYPE_H
#define CPP_TMP_STUDY_TYPE_H


// identity metafunction
template <typename T>
struct identity
{
    using type = T;
};


// utility type alias for convenience
template <typename T>
using type_is = identity<T>;


// type list
template <typename... T>
struct type_list
{ };


// type list alias for convenience
template <typename... T>
using TL = type_list<T...>;


#endif // CPP_TMP_STUDY_TYPE_H
