

#pragma once

#include "CommonMacros.h"


#include <string_view>
#include <unordered_map>
#include <array>


namespace QuirkEditor {

    // ============================================================================================================================
    //                                           ENUM_REGISTRY SYSTEM USAGE GUIDE
    // ============================================================================================================================
    //
    // 1. Registering an enum:
    //
    // REGISTER_ENUM(MyType, "TypeName",
    //    (Val1, "Val1Name"),
    //    (Val2, "Val2Name")
    //    ...
    // )
    //
    // - MyType:           The enum type to register.
    // - "TypeName":       Title/Display name of the enum type.
    // - Val1:             The value in the enum.
    // - Val1Name:         Title/Display name of the respective enum value.
    //
    // Example:
    //
    // REGISTER_ENUM(Color, "Color",
    //     (Red, "Red"),
    //     (Green, "Green"),
    //     (Blue, "Blue")
    // )
    //
    //
    // 2. EnumRegistry usage:
    //
    // constexpr size_t count         = QuirkEditor::EnumRegistry<Color>::ValueCount;
    // constexpr std::string_view str = QuirkEditor::EnumRegistry<Color>::ToString(Color::Blue);
    // 
    // Color val  = QuirkEditor::EnumRegistry<Color>::FromString(str);
    // auto names = QuirkEditor::EnumRegistry<Color>::GetValueNamesCStr();
    // 
    // 
    // ============================================================================================================================
    // ============================================================================================================================


    // Fallback EnumRegistry<EnumType> stub (used when no reflection is registered).
    // NOTE: No static_assert here!
    // In MSVC, even SFINAE/concepts-based checks like IsEnumRegistered
    // cause eager instantiation, which would always trigger the static_assert.
    // Reflection validity should be checked separately using IsEnumRegistered<T>.
    template<typename EnumType>
    struct EnumRegistry {
        // static_assert(false, "No Registered Enum!"); // Do not enable
    };



    // Default: not complex
    template<typename>
    struct IsEnumRegistered : std::false_type {};

    // Specialized: if EnumRegistry<T>::RegisteredType exists and matches T, it's a reflected enum
    template<typename T>
        requires std::is_same_v<T, typename EnumRegistry<T>::RegisteredType>
    struct IsEnumRegistered< T > : std::true_type {};

    template<typename T>
    constexpr bool HasEnumReflection = IsEnumRegistered<T>::value;




#define ENUM_STR_TO_VAL_PAIR(...)                                                                                          \
        { TUPLE_GET_SECOND(__VA_ARGS__),  RegisteredType::TUPLE_GET_FIRST(__VA_ARGS__) }

#define ENUM_VAL_TO_STR_PAIR(...)                                                                                          \
        case RegisteredType::TUPLE_GET_FIRST(__VA_ARGS__):   return TUPLE_GET_SECOND(__VA_ARGS__);



#define REGISTER_ENUM(Enum, EnumTitle, ...)                                                                                \
    template<>                                                                                                             \
    struct ::QuirkEditor::EnumRegistry<Enum> {                                                                             \
        using RegisteredType = Enum;                                                                                       \
                                                                                                                           \
        static constexpr std::string_view EnumName       = EnumTitle;                                                      \
        static constexpr std::array       ValueNamesCStr = { FOR_EACH_SEP_COMMA(TUPLE_GET_SECOND, __VA_ARGS__) };          \
        static constexpr size_t           ValueCount     = ValueNamesCStr.size();                                          \
                                                                                                                           \
        static inline const std::unordered_map<std::string_view, RegisteredType> StringToValueMapping{                     \
            FOR_EACH_SEP_COMMA(ENUM_STR_TO_VAL_PAIR, __VA_ARGS__)                                                          \
        };                                                                                                                 \
                                                                                                                           \
        static constexpr auto& GetValueNamesCStr() {                                                                       \
            return ValueNamesCStr;                                                                                         \
        }                                                                                                                  \
                                                                                                                           \
        static RegisteredType FromString(std::string_view value) {                                                         \
            auto val = StringToValueMapping.find(value);                                                                   \
            if (val != StringToValueMapping.end())                                                                         \
                return val->second;                                                                                        \
                                                                                                                           \
            return static_cast<RegisteredType>(0);                                                                         \
        }                                                                                                                  \
                                                                                                                           \
        static constexpr std::string_view ToString(RegisteredType value) {                                                 \
            switch (value) {                                                                                               \
                FOR_EACH(ENUM_VAL_TO_STR_PAIR, __VA_ARGS__)                                                                \
            }                                                                                                              \
            return "Unknown";                                                                                              \
        }                                                                                                                  \
    };

}
