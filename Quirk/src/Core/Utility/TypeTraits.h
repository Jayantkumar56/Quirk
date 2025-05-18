

#pragma once

#include <type_traits>
#include <filesystem>

namespace Quirk {

    template<typename T>
    struct AlwaysFalse : std::false_type {};

    template<typename T>
    inline constexpr bool AlwaysFalse_V = AlwaysFalse<T>::value;

    template<typename T>
    using RemoveAllWrapperTypes_T = std::remove_cvref_t<std::remove_pointer_t<std::remove_cvref_t<T>>>;



    template<typename ...Args>
    struct FirstNonVoid {};

    template<>
    struct FirstNonVoid<> { using type = void; };

    template<typename T, typename... Rest>
    struct FirstNonVoid<T, Rest...> {
        using type = std::conditional_t<std::is_void_v<T>, typename FirstNonVoid<Rest...>::type, T>;
    };

    template<typename ...Args>
    using FirstNonVoid_T = typename FirstNonVoid<Args...>::type;



    template<typename T>
    concept PathType = requires(T t) {
        { std::filesystem::path(t) };
    };

    template<template<typename> typename Condition, typename T>
    concept ConditionTrait = requires {
        { Condition<T>::value } -> std::convertible_to<bool>;
    };


    //=============================================================================================================================
    //--------- Pointer Detection -------------------------------------------------------------------------------------------------

    template<typename T>
    struct IsSmartPointer : std::false_type {};

    template<typename T>
    struct IsSmartPointer<std::shared_ptr<T>> : std::true_type {};

    template<typename T>
    struct IsSmartPointer<std::unique_ptr<T>> : std::true_type {};

    template<typename T>
    constexpr bool IsSmartPointer_V = IsSmartPointer<T>::value;

    template<typename T>                                                     // Concept
    concept SmartPointer = IsSmartPointer_V<T>;



    template<typename T>
    struct IsRawPointer : std::is_pointer<T> {};

    template<typename T>
    constexpr bool IsRawPointer_V = IsRawPointer<T>::value;

    template<typename T>                                                     // Concept
    concept RawPointer = IsRawPointer_V<T>;



    template<typename T>
    struct IsPointer : std::bool_constant<IsRawPointer_V<T> || IsSmartPointer_V<T>> {};

    template<typename T>
    constexpr bool IsPointer_V = IsPointer<T>::value;

    template<typename T>                                                     // Concept
    concept Pointer = IsPointer_V<T>;



    template<typename T>
    struct PointingType { using type = std::remove_pointer_t<T>; };

    template<typename T>
    struct PointingType<std::shared_ptr<T>> { using type = T; };

    template<typename T>
    struct PointingType<std::unique_ptr<T>> { using type = T; };

    template<typename T>
    using PointingType_T = typename PointingType<T>::type;

    //_____________________________________________________________________________________________________________________________



    //=============================================================================================================================
    //--------- Type List API -----------------------------------------------------------------------------------------------------

    template<typename ...Types>
    struct TypeList {
        template<typename Function, typename ...Args>
        static void ForEach(Function&& func, Args&& ...args) {
            (func.template operator() < Types > (args...), ...);
        }

        template<typename Function, typename ...Args>
        static decltype(auto) InvokeWithTypes(Function&& func, Args&& ...args) {
            return std::forward<Function>(func).template operator() < Types... > (std::forward<Args>(args)...);
        }
    };


    template<typename Element>
    struct Front { using type = Element; };

    template<typename Head, typename ...Tail>
    struct Front < TypeList<Head, Tail...> > { using type = Head; };

    template<typename List>
    using Front_T = typename Front<List>::type;



    template<typename Element>
    struct Back { using type = Element; };

    template<typename Tail>
    struct Back<TypeList<Tail>> { using type = Tail; };

    template<typename Head, typename ...Tail>
    struct Back<TypeList<Head, Tail...>> { using type = typename Back<TypeList<Tail...>>::type; };

    template<typename List>
    using Back_T = typename Back<List>::type;



    template<typename Element1, typename Element2>
    struct PushFront { using type = TypeList<Element2, Element1>; };

    template<typename Element, typename ...Elements>
    struct PushFront<TypeList<Elements...>, Element> { using type = TypeList<Element, Elements...>; };

    template<typename List, typename Element>
    using PushFront_T = typename PushFront<List, Element>::type;



    template<typename Element1, typename Element2>
    struct PushBack { using type = TypeList<Element1, Element2>; };

    template<typename Element, typename ...Elements>
    struct PushBack<TypeList<Elements...>, Element> { using type = TypeList<Elements..., Element>; };

    template<typename List, typename Element>
    using PushBack_T = typename PushBack<List, Element>::type;



    template<template<typename> typename Condition, typename List, typename Element>
    requires ConditionTrait<Condition, Element>
    struct PushBackIf { using type = List; };

    template<template<typename> typename Condition, typename Element, typename ...Elements>
    requires ConditionTrait<Condition, Element> && Condition<Element>::value
    struct PushBackIf<Condition, TypeList<Elements...>, Element> { using type = PushBack_T<TypeList<Elements...>, Element>; };

    template<template<typename> typename Condition, typename List, typename Element>
    requires ConditionTrait<Condition, Element>
    using PushBackIf_T = typename PushBackIf<Condition, List, Element>::type;



    template<template<typename> typename Condition, typename ListResult, typename InputList>
    struct FilterTypesImpl {};

    template<template<typename> typename Condition, typename ListResult>
    struct FilterTypesImpl<Condition, ListResult, TypeList<>> { using type = ListResult; };

    template<template<typename> typename Condition, typename ListResult, typename Element, typename... Elements>
    struct FilterTypesImpl<Condition, ListResult, TypeList<Element, Elements...>> {
    private:
        using TailResult = typename FilterTypesImpl<Condition, ListResult, TypeList<Elements...>>::type;
    public:
        using type = std::conditional_t<
            ConditionTrait<Condition, Element> && Condition<Element>::value,
            PushFront_T<TailResult, Element>,
            TailResult
        >;
    };

    template<template<typename> typename Condition, typename List>
    using FilterTypes_T = typename FilterTypesImpl<Condition, TypeList<>, List>::type;

    //_____________________________________________________________________________________________________________________________

}

