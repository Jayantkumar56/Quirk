

#pragma once

#include "SmartPointer.h"
#include "View.h"

#include <type_traits>
#include <filesystem>
#include <tuple>

namespace Quirk {

    //=============================================================================================================================
    //--------- General Traits ----------------------------------------------------------------------------------------------------

    template<typename T>
    struct IsNonVoid { static constexpr bool Value = std::negation_v<std::is_void<T>>; };

    template<typename T>
    constexpr bool IsNonVoid_V = IsNonVoid<T>::Value;



    template<bool B>
    struct BoolConstant { static constexpr bool Value = B; };

    using TrueType  = BoolConstant<true>;
    using FalseType = BoolConstant<false>;

    constexpr bool TrueType_V  = TrueType::Value;
    constexpr bool FalseType_V = FalseType::Value;



    template<typename T>
    struct AlwaysFalse : FalseType {};

    template<typename T>
    inline constexpr bool AlwaysFalse_V = AlwaysFalse<T>::Value;

    // removes cv qualifiers and pointer from the type
    template<typename T>
    using RemoveAllWrapperTypes_T = std::remove_cvref_t<std::remove_pointer_t<std::remove_cvref_t<T>>>;



    template<typename T>
    concept PathType = requires(T t) { { std::filesystem::path(t) }; };

    template<typename T>
    concept TupleType = requires { typename std::tuple_size<std::remove_reference_t<T>>; };

    template<template<typename> typename Condition, typename T>
    concept ConditionTrait = requires { { Condition<T>::Value } -> std::convertible_to<bool>; };

    //_____________________________________________________________________________________________________________________________



    //=============================================================================================================================
    //--------- Compile time string literal ---------------------------------------------------------------------------------------

    template <size_t N>
    struct StringLiteral {
        char value[N]{};

        constexpr StringLiteral(const char(&str)[N]) noexcept {
            for (size_t i = 0; i < N; ++i)
                value[i] = str[i];
        }

        constexpr const char* Data()                    const noexcept { return value; }
        constexpr std::string_view View()               const noexcept { return { value, N }; }
        constexpr size_t           Size()               const noexcept { return N - 1; }
        constexpr char             operator[](size_t i) const noexcept { return value[i]; }

        constexpr bool operator==(const StringLiteral& other) const noexcept {
            for (size_t i = 0; i < N; ++i) {
                if (value[i] != other.value[i])
                    return false;
            }

            return true;
        }
    };

    template <size_t N>
    StringLiteral(const char(&str)[N]) -> StringLiteral<N>;

    //_____________________________________________________________________________________________________________________________



    //=============================================================================================================================
    //--------- Enum Traits -------------------------------------------------------------------------------------------------------

    template<auto Val>
    struct IsEnumValue { static constexpr bool Value = std::is_enum_v<std::decay_t<decltype(Val)>>; };

    template<auto Val>
    constexpr bool IsEnumValue_V = IsEnumValue<Val>::Value;

    template<auto Val>
    concept EnumValue = IsEnumValue_V<Val>;



    template<auto Val1, auto Val2, auto ...ValList>
    requires EnumValue<Val1> && EnumValue<Val2> && (EnumValue<ValList> && ...)
    struct SameEnumType {
        using Type1 = std::decay_t<decltype(Val1)>;
        using Type2 = std::decay_t<decltype(Val2)>;
        static constexpr bool Value = std::is_same_v<Type1, Type2> && (SameEnumType<Val1, ValList>::Value && ...);
    };

    template<auto Val1, auto Val2, auto ...ValList>
    constexpr bool SameEnumType_V = SameEnumType<Val1, Val2, ValList...>::Value;

    template<auto Val1, auto Val2, auto ...ValList>
    concept SameEnum = SameEnumType_V<Val1, Val2, ValList...>;



    template<auto Key, auto ListVal1, auto ...List>
    requires SameEnum<Key, ListVal1, List...>
    struct IsEnumValuePresent { static constexpr bool Value = (Key == ListVal1) || ((Key == List) || ...); };

    template<auto Key, auto ListVal1, auto ...List>
    requires SameEnum<Key, ListVal1, List...>
    constexpr bool IsEnumValuePresent_V = IsEnumValuePresent<Key, ListVal1, List...>::Value;



    template<auto Type1, auto ...List>
    requires EnumValue<Type1> && (EnumValue<List> && ...)
    struct DuplicatesInEnumValues {
        static constexpr bool Value = ((Type1 == List) || ...) || DuplicatesInEnumValues<List...>::Value;
    };

    template<auto Type>
    requires EnumValue<Type>
    struct DuplicatesInEnumValues<Type> : FalseType {};

    template<auto Type1, auto ...List>
    requires EnumValue<Type1> && (EnumValue<List> && ...)
    constexpr bool DuplicatesInEnumValues_V = DuplicatesInEnumValues<List...>::Value;

    //_____________________________________________________________________________________________________________________________



    //=============================================================================================================================
    //--------- View Detection ----------------------------------------------------------------------------------------------------

    template<typename T>
    struct IsView : FalseType {};

    template<typename T>
    struct IsView<View<T>> : TrueType {};

    template<typename T>
    constexpr bool IsView_V = IsView<T>::Value;

    template<typename T>
    concept ViewType = IsView_V<T>;

    //_____________________________________________________________________________________________________________________________



    //=============================================================================================================================
    //--------- Pointer Detection -------------------------------------------------------------------------------------------------

    // NOTE:
    // 
    // - must use _V versions, the raw traits are not meant to be used
    // 
    // - _V version applies remove_cvref_t to normalize the types
    //   thus traits work correctly with references and qualifiers
    //   The base trait remains unnormalized for clarity and specialization control
    // 
    // - concepts uses _V internally thus it automatically normalize the types

    template<typename T>
    struct IsSmartPointer : FalseType {};

    template<typename T>
    struct IsSmartPointer<std::shared_ptr<T>> : TrueType {};

    template<typename T>
    struct IsSmartPointer<std::unique_ptr<T>> : TrueType {};

    template<typename T>
    constexpr bool IsSmartPointer_V = IsSmartPointer<std::remove_cvref_t<T>>::Value;

    template<typename T>                                                     // Concept
    concept SmartPointer = IsSmartPointer_V<T>;



    template<typename T>
    struct IsRawPointer { static constexpr bool Value = std::is_pointer_v<T>; };

    template<typename T>
    constexpr bool IsRawPointer_V = IsRawPointer<std::remove_cvref_t<T>>::Value;

    template<typename T>                                                     // Concept
    concept RawPointer = IsRawPointer_V<T>;



    template<typename T>
    struct IsPointer { static constexpr bool Value = IsRawPointer_V<T> || IsSmartPointer_V<T>; };

    template<typename T>
    constexpr bool IsPointer_V = IsPointer<std::remove_cvref_t<T>>::Value;

    template<typename T>                                                     // Concept
    concept Pointer = IsPointer_V<T>;



    template<typename T>
    struct PointingType { using Type = std::remove_pointer_t<T>; };

    template<typename T>
    struct PointingType<std::shared_ptr<T>> { using Type = T; };

    template<typename T>
    struct PointingType<std::unique_ptr<T>> { using Type = T; };

    template<typename T>
    using PointingType_T = typename PointingType<std::remove_cvref_t<T>>::Type;


    // Pointer type conversion

    template<Pointer To, Pointer From>
    To ConvertPointer(From&& ptr) noexcept {
        using ToType   = std::remove_cvref_t<To>;
        using FromType = std::remove_cvref_t<From>;

        if constexpr (std::is_same_v<ToType, FromType>) {
            return std::forward<FromType>(ptr);
        }
        else {
            if constexpr (IsRawPointer_V<ToType>) {
                return std::forward<FromType>(ptr).get();
            }
            else if constexpr (IsSmartPointer_V<FromType>) {
                return ToType(std::forward<FromType>(ptr).get());
            }
            else {
                return ToType(std::forward<FromType>(ptr));
            }
        }
    }

    //_____________________________________________________________________________________________________________________________



    //=============================================================================================================================
    //--------- Member type detection ---------------------------------------------------------------------------------------------

    // getter for member
    template<typename T, auto Getter, bool DirectMemberAccess = false>
    struct PropertyGetterTypeImpl { using Type = decltype((std::declval<T>().*Getter)()); };

    // member pointer 
    template<typename T, auto Member>
    struct PropertyGetterTypeImpl<T, Member, true> { using Type = decltype(std::declval<T>().*Member); };

    template<typename T, auto Getter, bool DirectMemberAccess>
    using PropertyType_T = typename PropertyGetterTypeImpl<T, Getter, DirectMemberAccess>::Type;

    template<typename T, auto Getter, bool DirectMemberAccess>
    using UnwrappedPropertyType_T = RemoveAllWrapperTypes_T<PropertyType_T<T, Getter, DirectMemberAccess>>;

    //_____________________________________________________________________________________________________________________________



    //=============================================================================================================================
    //--------- Type Pack Traits --------------------------------------------------------------------------------------------------

    template<typename ...Args>
    struct FirstNonVoid {};

    template<>
    struct FirstNonVoid<> { using Type = void; };

    template<typename T, typename... Rest>
    struct FirstNonVoid<T, Rest...> {
        using Type = std::conditional_t<std::is_void_v<T>, typename FirstNonVoid<Rest...>::Type, T>;
    };

    template<typename ...Types>
    using FirstNonVoid_T = typename FirstNonVoid<Types...>::Type;



    template<typename Key, typename ListVal1, typename ...List>
    struct IsPresent { static constexpr bool Value = (std::is_same_v<Key, ListVal1> || (std::is_same_v<Key, List> || ...)); };

    template<typename Key, typename ListVal1, typename ...List>
    constexpr bool IsPresent_V = IsPresent<Key, ListVal1, List>::Value;



    template<typename Type1, typename ...List>
    struct DuplicatesInPack {
        static constexpr bool Value = (std::is_same_v<Type1, List> || ...) || (DuplicatesInPack<List...>::Value);
    };

    template<typename Type>
    struct DuplicatesInPack<Type> : FalseType {};

    template<typename Type1, typename ...List>
    constexpr bool DuplicatesInPack_V = DuplicatesInPack<List...>::Value;
    
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



    template<typename List>
    struct TypeListSize;

    template<typename ...Elements>
    struct TypeListSize<TypeList<Elements...>> { static constexpr size_t Value = sizeof...(Elements); };

    template<typename List>
    constexpr size_t TypeListSize_V = TypeListSize<List>::Value;



    template<typename List1, typename List2>
    struct IsSameTypeList : FalseType {};

    template<typename ...Ts, typename ...Us>
    struct IsSameTypeList<TypeList<Ts...>, TypeList<Us...>> {
        static constexpr bool Value = std::conditional_t<
            TypeListSize_V<TypeList<Ts...>> == TypeListSize_V<TypeList<Us...>>,
            std::conjunction<std::is_same<Ts, Us>...>,
            std::false_type
        >::value;
    };

    template<typename List1, typename List2>
    constexpr bool IsSameTypeList_V = IsSameTypeList<List1, List2>::Value;



    template<typename T, template<typename> typename TupleGetter, typename List>
    struct Creator;

    template<typename T, template<typename> typename TupleGetter, typename... Elements>
    struct Creator<T, TupleGetter, TypeList<Elements...>> {
        template<typename... Args>
        static T Create(Args&&... args) {
            return T(TupleGetter<Elements>::Get(std::forward<Args>(args)...)...);
        }
    };

    template<typename T, template<typename> typename TupleGetter, typename List, typename... Args>
    T ConstructFromTypeList(Args&&... args) {
        return Creator<T, TupleGetter, List>::Create(std::forward<Args>(args)...);
    }



    template<typename Element>
    struct Front { using Type = Element; };

    template<typename Head, typename ...Tail>
    struct Front < TypeList<Head, Tail...> > { using Type = Head; };

    template<typename List>
    using Front_T = typename Front<List>::Type;



    template<typename Element>
    struct Back { using Type = Element; };

    template<typename Tail>
    struct Back<TypeList<Tail>> { using Type = Tail; };

    template<typename Head, typename ...Tail>
    struct Back<TypeList<Head, Tail...>> { using Type = typename Back<TypeList<Tail...>>::Type; };

    template<typename List>
    using Back_T = typename Back<List>::Type;



    template<typename Element1, typename Element2>
    struct PushFront { using Type = TypeList<Element2, Element1>; };

    template<typename Element, typename ...Elements>
    struct PushFront<TypeList<Elements...>, Element> { using Type = TypeList<Element, Elements...>; };

    template<typename List, typename Element>
    using PushFront_T = typename PushFront<List, Element>::Type;



    template<typename Element1, typename Element2>
    struct PushBack { using Type = TypeList<Element1, Element2>; };

    template<typename Element, typename ...Elements>
    struct PushBack<TypeList<Elements...>, Element> { using Type = TypeList<Elements..., Element>; };

    template<typename List, typename Element>
    using PushBack_T = typename PushBack<List, Element>::Type;



    template<template<typename> typename Condition, typename List, typename Element>
    requires ConditionTrait<Condition, Element>
    struct PushBackIf { using Type = List; };

    template<template<typename> typename Condition, typename Element, typename ...Elements>
    requires ConditionTrait<Condition, Element> && Condition<Element>::Value
    struct PushBackIf<Condition, TypeList<Elements...>, Element> { using Type = PushBack_T<TypeList<Elements...>, Element>; };

    template<template<typename> typename Condition, typename List, typename Element>
    requires ConditionTrait<Condition, Element>
    using PushBackIf_T = typename PushBackIf<Condition, List, Element>::Type;



    template<template<typename> typename Condition, typename ListResult, typename InputList>
    struct FilterTypesImpl {};

    template<template<typename> typename Condition, typename ListResult>
    struct FilterTypesImpl<Condition, ListResult, TypeList<>> { using Type = ListResult; };

    template<template<typename> typename Condition, typename ListResult, typename Element, typename... Elements>
    struct FilterTypesImpl<Condition, ListResult, TypeList<Element, Elements...>> {
    private:
        using TailResult = typename FilterTypesImpl<Condition, ListResult, TypeList<Elements...>>::Type;
    public:
        using Type = std::conditional_t<
            ConditionTrait<Condition, Element> && Condition<Element>::Value,
            PushFront_T<TailResult, Element>,
            TailResult
        >;
    };

    template<template<typename> typename Condition, typename List>
    using FilterTypes_T = typename FilterTypesImpl<Condition, TypeList<>, List>::Type;

    //_____________________________________________________________________________________________________________________________



    //=============================================================================================================================
    //--------- Inheritance Traits ------------------------------------------------------------------------------------------------

    enum class InheritanceVisibility {
        Public,
        Protected,
        Private
    };



    template<InheritanceVisibility Visibility, typename List>
    struct InheritFromTypeList;

    template<typename... Elements>
    struct InheritFromTypeList<InheritanceVisibility::Public, TypeList<Elements...>> : public Elements... {
        template<TupleType ...Tuples>
        requires (sizeof...(Tuples) == sizeof...(Elements))
        InheritFromTypeList(Tuples&& ...tuples) : 
                Elements(std::make_from_tuple<Elements>(std::forward<Tuples>(tuples)))...
        {}
    };

    template<typename... Elements>
    struct InheritFromTypeList<InheritanceVisibility::Protected, TypeList<Elements...>> : protected Elements... {
        template<TupleType ...Tuples>
        requires (sizeof...(Tuples) == sizeof...(Elements))
        InheritFromTypeList(Tuples&& ...tuples) :
                Elements(std::make_from_tuple<Elements>(std::forward<Tuples>(tuples)))...
        {}
    };

    template<typename... Elements>
    struct InheritFromTypeList<InheritanceVisibility::Private, TypeList<Elements...>> : private Elements... {
        template<TupleType ...Tuples>
        requires (sizeof...(Tuples) == sizeof...(Elements))
        InheritFromTypeList(Tuples&& ...tuples) :
                Elements(std::make_from_tuple<Elements>(std::forward<Tuples>(tuples)))...
        {}
    };
    
    //_____________________________________________________________________________________________________________________________

}
