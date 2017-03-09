#ifndef CPP_TMP_STUDY_SIZEOF_H
#define CPP_TMP_STUDY_SIZEOF_H


// returns number of types in the TypeListContainer.
template <typename TypeListContainer>
struct length;

template
<
    template <typename...> class TypeListContainer,
    typename... T
>
struct length<TypeListContainer<T...>>
{
    // NOTE: member variable value's type has const qualifier here.
    constexpr static auto value = sizeof...(T);
};


#endif // CPP_TMP_STUDY_SIZEOF_H
