

#pragma once

#include "CommonMacros.h"

#include "Core/Utility/TypeTraits.h"

#include <string_view>
#include <type_traits>


namespace QuirkEditor {

    // ============================================================================================================================
    //                                           REFLECTION SYSTEM USAGE GUIDE
    // ============================================================================================================================
    //
    // 1. Registering a reflected class:
    //
    // REGISTER_REFLECTION(MyType, "TypeName",
    //     (PropertyName, Getter, Setter, AccessMode),
    //     ...
    // )
    //
    // - MyType:           The type to reflect.
    // - "TypeName":       Title/Display name.
    // - PropertyName:     Unique identifier for the property struct.
    // - Getter/Setter:    Member function pointers (no & needed).
    // - AccessMode:       Use PropertyAccess::ReadOnly, ReadWrite, etc.
    //
    // Example:
    //
    // REGISTER_REFLECTION(Player, "Player",
    //     (Health, GetHealth, SetHealth, PropertyAccess::ReadWrite),
    //     (Name,   GetName,   SetName,   PropertyAccess::ReadWrite)
    // )
    //
    //
    // 2. Reflecting over properties:
    //
    // Reflect<Player>::ForEach([&]<typename Property>() {
    //     auto value = Property::Get(&player);
    //     std::cout << Property::PropertyName << " = " << value << "\n";
    // });
    //
    //
    // 3. Enums (optional):
    //
    // REGISTER_ENUM(MyEnumType, "EnumName",
    //     (EnumVal1, "Label 1"),
    //     (EnumVal2, "Label 2")
    // )
    //
    // - Adds support for EnumRegistry<MyEnumType>::ToString(value)
    //   and FromString("Label 1").
    //
    // ----------------------------------------------------------------------------------------------------------------------------
    // Notes:
    // - Avoid naming a property "Name", as it may collide with internal identifiers.
    // - All macros resolve to valid C++11+ templates; however, IntelliSense may struggle.
    // - Only getter is required for ReadOnly. Setter required for ReadWrite.
    // - Use only 4-element tuples for REGISTER_REFLECTION.
    // ============================================================================================================================
    // ============================================================================================================================

    enum class PropertyAccess {
        ReadWrite,
        ReadOnly
    };

    // Fallback Reflect<T> stub (used when no reflection is registered).
    // NOTE: No static_assert here!
    // In MSVC, even SFINAE/concepts-based checks like IsComplexReflectable
    // cause eager instantiation, which would always trigger the static_assert.
    // Reflection validity should be checked separately using IsComplexReflectable<T>.
    template<typename T>
    struct Reflect {
        // static_assert(false, "No Registered Reflection!"); // Do not enable
    };



    // Default: not complex type
    template<typename>
    struct IsComplexReflectable : std::false_type {};

    // Specialized: if Reflect<T>::ReflectingType exists and matches T, then it's a complex (reflected) type
    template<typename T>
        requires std::is_same_v<T, typename Reflect<T>::ReflectingType>
    struct IsComplexReflectable<T> : std::true_type {};

    template<typename T>
    constexpr bool HasReflection = IsComplexReflectable<T>::value;



#define REGISTER_PROPERTY_ACCESS(PropName, Getter, Setter, AccessMode)                                                      \
    struct PropName {                                                                                                       \
        using Type = std::remove_cvref_t<decltype(((ReflectingType*)nullptr)->Getter())>;                                   \
                                                                                                                            \
        static constexpr std::string_view PropertyName  = #PropName;                                                        \
        static constexpr auto   Accessibility           = AccessMode;                                                       \
        static constexpr bool   Writable                = (Accessibility == ::QuirkEditor::PropertyAccess::ReadWrite);      \
                                                                                                                            \
        template<typename Obj, typename... Args>                                                                            \
        requires (Writable)                                                                                                 \
        static inline void Set(Obj&& obj, Args&&... args) {                                                                 \
            static_assert(                                                                                                  \
                std::is_same_v<std::remove_cvref_t<Obj>, ReflectingType> ||                                                 \
                std::is_same_v<std::remove_cvref_t<Obj>, ReflectingType*>,                                                  \
                "Wrong object type passed to Set()"                                                                         \
            );                                                                                                              \
                                                                                                                            \
            if constexpr (std::is_pointer_v<std::decay_t<Obj>>)                                                             \
                (*obj).Setter(std::forward<Args>(args)...);                                                                 \
            else                                                                                                            \
                obj.Setter(std::forward<Args>(args)...);                                                                    \
        }                                                                                                                   \
                                                                                                                            \
        template<typename Obj>                                                                                              \
        static inline decltype(auto) Get(Obj&& obj) {                                                                       \
            static_assert(                                                                                                  \
                std::is_same_v<std::remove_cvref_t<Obj>, ReflectingType> ||                                                 \
                std::is_same_v<std::remove_cvref_t<Obj>, ReflectingType*>,                                                  \
                "Wrong object type passed to Set()"                                                                         \
            );                                                                                                              \
                                                                                                                            \
            if constexpr (std::is_pointer_v<std::decay_t<Obj>>)                                                             \
                return (*obj).Getter();                                                                                     \
            else                                                                                                            \
                return obj.Getter();                                                                                        \
        }                                                                                                                   \
    };

    // deligating to multiple macro to delay the macro expansion for next phase
#define REGISTER_PROP_TUPLE_4(...) REGISTER_PROPERTY_ACCESS(__VA_ARGS__)
#define REGISTER_PROP_TUPLE(t)     REGISTER_PROP_TUPLE_4(EXPAND_TUPLE_4(t))

#define REGISTER_REFLECTION(TYPE_, TITLE_, ...)                                                                             \
    template<>                                                                                                              \
    struct ::QuirkEditor::Reflect<TYPE_> {                                                                                  \
        using ReflectingType  = TYPE_;                                                                                      \
        static constexpr std::string_view TypeName = TITLE_;                                                                \
                                                                                                                            \
        FOR_EACH(REGISTER_PROP_TUPLE, __VA_ARGS__)                                                                          \
                                                                                                                            \
        using PropertyList = Quirk::TypeList <                                                                              \
            FOR_EACH_SEP_COMMA(TUPLE_GET_FIRST_4, __VA_ARGS__)                                                              \
        >;                                                                                                                  \
                                                                                                                            \
        template<typename Function, typename ...Args>                                                                       \
        static void ForEach(Function&& func, Args&& ...args) {                                                              \
            PropertyList::ForEach(std::forward<Function>(func), std::forward<Args>(args)...);                               \
        }                                                                                                                   \
    };

}
