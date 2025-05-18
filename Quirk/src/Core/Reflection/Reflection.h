

#pragma once

#include "CommonMacros.h"
#include "Core/Utility/TypeTraits.h"

#include <string_view>
#include <type_traits>


namespace Quirk {

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
    // - PropertyFlags_:   Use PropertyFlags::Editable, PropertyFlags::Serializable, etc.
    //
    // Example:
    //
    // REGISTER_REFLECTION(HealthBar, "HealthBar", (CONSTRUCTOR),
    //     (IsHealthy,    GetIsHealthy,    SetIsHealthy,    Quirk::PropertyFlag::Editable | Quirk::PropertyFlag::Serializable),
    //     (HealthStatus, GetHealthStatus, SetHealthStatus, Quirk::PropertyFlag::Editable | Quirk::PropertyFlag::Serializable)
    // )
    //
    // 
    // 2. In case of ReadOnly property put a DUMMY placeholder in set
    // 
    // Example:
    // 
    // REGISTER_REFLECTION(HealthBar, "HealthBar", (CONSTRUCTOR),
    //     (IsHealthy,    GetIsHealthy,    DUMMY,           Quirk::PropertyFlag::Serializable                                ),
    //     (HealthStatus, GetHealthStatus, SetHealthStatus, Quirk::PropertyFlag::Editable | Quirk::PropertyFlag::Serializable)
    // )
    //
    //
    // ----------------------------------------------------------------------------------------------------------------------------
    // Notes:
    // - Avoid naming a property "Name", as it may collide with internal identifiers.
    // - All macros resolve to valid  templates, IntelliSense may struggle, and not work in generic ForEach.
    // - Only getter is required for ReadOnly. Setter required for ReadWrite.
    // - Use only 4-element tuples for REGISTER_REFLECTION.
    // ============================================================================================================================
    // ============================================================================================================================


    enum class PropertyFlag : uint32_t {
        None         = 0,
        Serializable = 1 << 0,
        Editable     = 1 << 1
    };

    constexpr bool HasPropertyFlag(uint32_t value, PropertyFlag flag) {
        return (value & static_cast<uint32_t>(flag)) != 0;
    }


    // Fallback Reflect<T> stub (used when no reflection is registered).
    // NOTE: No static_assert here!
    // In MSVC, even SFINAE/concepts-based checks like IsComplexReflectable
    // cause eager instantiation, which would always trigger the static_assert.
    // Reflection validity should be checked separately using IsComplexReflectable<T>.
    template<typename T>
    class Reflect {
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
    constexpr bool HasReflection_V = IsComplexReflectable<T>::value;

    template<class T>
    concept ComplexReflectable = HasReflection_V<T>;



    //=============================================================================================================================
    //--------- Property Registry for Reflecting Type -----------------------------------------------------------------------------

#define REGISTER_PROPERTY_ACCESS(PropName_, Getter_, Setter_, PropertyFlags_)                                               \
    struct PropName_ {                                                                                                      \
        using Type = RemoveAllWrapperTypes_T<decltype(((ReflectingType*)nullptr)->Getter_())>;                              \
                                                                                                                            \
        static constexpr std::string_view PropertyName  = #PropName_;                                                       \
        static constexpr uint32_t         PropFlags     = static_cast<uint32_t>(PropertyFlags_);                            \
        static constexpr bool             Writable      = HasPropertyFlag(PropertyFlags_, ::Quirk::PropertyFlag::Editable); \
                                                                                                                            \
        template<typename Obj, typename... Args>                                                                            \
        requires (Writable)                                                                                                 \
        static void Set(Obj&& obj, Args&&... args) {                                                                        \
            static_assert(                                                                                                  \
                std::is_same_v<std::remove_cvref_t<Obj>, ReflectingType> ||                                                 \
                std::is_same_v<std::remove_cvref_t<Obj>, ReflectingType*>,                                                  \
                "Wrong object type passed to Set()"                                                                         \
            );                                                                                                              \
                                                                                                                            \
            if constexpr (std::is_pointer_v<std::decay_t<Obj>>)                                                             \
                (*obj).Setter_(std::forward<Args>(args)...);                                                                \
            else                                                                                                            \
                obj.Setter_(std::forward<Args>(args)...);                                                                   \
        }                                                                                                                   \
                                                                                                                            \
        template<typename Obj>                                                                                              \
        static decltype(auto) Get(Obj&& obj) {                                                                              \
            static_assert(                                                                                                  \
                std::is_same_v<std::remove_cvref_t<Obj>, ReflectingType> ||                                                 \
                std::is_same_v<std::remove_cvref_t<Obj>, ReflectingType*>,                                                  \
                "Wrong object type passed to Get()"                                                                         \
            );                                                                                                              \
                                                                                                                            \
            if constexpr (std::is_pointer_v<std::decay_t<Obj>>)                                                             \
                return (*obj).Getter_();                                                                                    \
            else                                                                                                            \
                return obj.Getter_();                                                                                       \
        }                                                                                                                   \
    };

    //_____________________________________________________________________________________________________________________________



    //=============================================================================================================================
    //--------- Factory Function Macros for Reflecting Type -----------------------------------------------------------------------

#define REGISTER_FACTORY(FACTORY_FUNC_NAME_, FACTORY_FUNC_)                                                                 \
    struct FACTORY_FUNC_NAME_ {                                                                                             \
        static auto Invoke(auto&&... args) { return FACTORY_FUNC_(std::forward<decltype(args)>(args)...); }                 \
    };

#define CONSTRUCTOR                       REGISTER_FACTORY( Create,            ReflectingType )
#define FACTORY(FACTORY_FUNC_)            REGISTER_FACTORY( CreateWithFactory, FACTORY_FUNC_  )
#define FACTORY_PTR(FACTORY_FUNC_)        REGISTER_FACTORY( CreatePtr,         FACTORY_FUNC_  )
#define FACTORY_REF(FACTORY_FUNC_)        REGISTER_FACTORY( CreateRef,         FACTORY_FUNC_  )
#define FACTORY_SCOPE(FACTORY_FUNC_)      REGISTER_FACTORY( CreateScope,       FACTORY_FUNC_  )



#define REGISTER_FACTORY_FUC_CHECK(FACTORY_FUNC_)                                                                           \
    template<typename T, typename = void>                                                                                   \
    struct FACTORY_FUNC_##OrVoid { using type = void; };                                                                    \
                                                                                                                            \
    template<typename T>                                                                                                    \
    struct FACTORY_FUNC_##OrVoid<T, std::void_t<typename T::FACTORY_FUNC_>> { using type = typename T::FACTORY_FUNC_; };


    REGISTER_FACTORY_FUC_CHECK( Create            )
    REGISTER_FACTORY_FUC_CHECK( CreateWithFactory )
    REGISTER_FACTORY_FUC_CHECK( CreatePtr         )
    REGISTER_FACTORY_FUC_CHECK( CreateRef         )
    REGISTER_FACTORY_FUC_CHECK( CreateScope       )
    
    //_____________________________________________________________________________________________________________________________
    


    //=============================================================================================================================
    //--------- Reflect on Type ---------------------------------------------------------------------------------------------------

    // deligating to multiple macro to delay the macro expansion for next phase
#define REGISTER_PROPERTY_HELPER(...)       REGISTER_PROPERTY_ACCESS(__VA_ARGS__)
#define REGISTER_PROPERTY_TUPLE(TUPLE_)     REGISTER_PROPERTY_HELPER(EXPAND_TUPLE_4(TUPLE_))

#define REGISTER_REFLECTION(TYPE_, TITLE_, FACTORY_FUNCS_, ...)                                                             \
    template<>                                                                                                              \
    class ::Quirk::Reflect<TYPE_> {                                                                                         \
    public:                                                                                                                 \
        using ReflectingType = TYPE_;                                                                                       \
                                                                                                                            \
        static constexpr std::string_view TypeName = TITLE_;                                                                \
        FOR_EACH(REGISTER_PROPERTY_TUPLE, __VA_ARGS__)                                                                      \
                                                                                                                            \
    private:                                                                                                                \
        using PropertyList = ::Quirk::TypeList < FOR_EACH_SEP_COMMA(TUPLE_GET_FIRST_4, __VA_ARGS__) >;                      \
                                                                                                                            \
        template<typename Property, ::Quirk::PropertyFlag Flag>                                                             \
        struct PropertyFlagCondition {                                                                                      \
            static constexpr bool value = ::Quirk::HasPropertyFlag(Property::PropFlags, Flag);                              \
        };                                                                                                                  \
                                                                                                                            \
        template<::Quirk::PropertyFlag FilterFlags>                                                                         \
        struct FilteredWithTypesInvoker {                                                                                   \
            template<typename Property>                                                                                     \
            using FilterCondition = PropertyFlagCondition<Property, FilterFlags>;                                           \
                                                                                                                            \
            template<typename Function, typename... Args>                                                                   \
            static decltype(auto) Invoke(Function&& func, Args&&... args) {                                                 \
                using FilteredProperties = ::Quirk::FilterTypes_T<FilterCondition, PropertyList>;                           \
                return FilteredProperties::InvokeWithTypes(std::forward<Function>(func), std::forward<Args>(args)...);      \
            }                                                                                                               \
        };                                                                                                                  \
                                                                                                                            \
        template<::Quirk::PropertyFlag FilterFlags>                                                                         \
        struct FilteredForEachInvoker {                                                                                     \
            template<typename Property>                                                                                     \
            using FilterCondition = PropertyFlagCondition<Property, FilterFlags>;                                           \
                                                                                                                            \
            template<typename Function, typename... Args>                                                                   \
            static void Invoke(Function&& func, Args&&... args) {                                                           \
                using FilteredProperties = ::Quirk::FilterTypes_T<FilterCondition, PropertyList>;                           \
                FilteredProperties::InvokeWithTypes(std::forward<Function>(func), std::forward<Args>(args)...);             \
            }                                                                                                               \
        };                                                                                                                  \
                                                                                                                            \
    public:                                                                                                                 \
        FOR_EACH(IDENTITY, UNPACK_TUPLE(FACTORY_FUNCS_))                                                                    \
                                                                                                                            \
        using CreateOrVoid_T            = typename ::Quirk::CreateOrVoid            <Reflect>::type;                        \
        using CreateWithFactoryOrVoid_T = typename ::Quirk::CreateWithFactoryOrVoid <Reflect>::type;                        \
        using CreatePtrOrVoid_T         = typename ::Quirk::CreatePtrOrVoid         <Reflect>::type;                        \
        using CreateRefOrVoid_T         = typename ::Quirk::CreateRefOrVoid         <Reflect>::type;                        \
        using CreateScopeOrVoid_T       = typename ::Quirk::CreateScopeOrVoid       <Reflect>::type;                        \
                                                                                                                            \
        template<typename Function, typename ...Args>                                                                       \
        static void ForEach(Function&& func, Args&& ...args) {                                                              \
            PropertyList::ForEach(std::forward<Function>(func), std::forward<Args>(args)...);                               \
        }                                                                                                                   \
                                                                                                                            \
        template<typename Function, typename ...Args>                                                                       \
        static decltype(auto) InvokeWithTypes(Function&& func, Args&& ...args) {                                            \
            return PropertyList::InvokeWithTypes(std::forward<Function>(func), std::forward<Args>(args)...);                \
        }                                                                                                                   \
                                                                                                                            \
        template<::Quirk::PropertyFlag FilterFlags, typename Function, typename... Args>                                    \
        static void ForEachFiltered(Function&& func, Args& ...args) {                                                       \
            FilteredForEachInvoker<FilterFlags>::Invoke(std::forward<Function>(func), std::forward<Args>(args)...);         \
        }                                                                                                                   \
                                                                                                                            \
        template<::Quirk::PropertyFlag FilterFlags, typename Function, typename... Args>                                    \
        static decltype(auto) InvokeWithTypesFiltered(Function&& func, Args& ...args) {                                     \
            return FilteredWithTypesInvoker<FilterFlags>::Invoke(std::forward<Function>(func), std::forward<Args>(args)...);\
        }                                                                                                                   \
                                                                                                                            \
        template<typename Function, typename ...Args>                                                                       \
        static void ForEachSerializable(Function&& func, Args&& ...args) {                                                  \
            FilteredForEachInvoker<::Quirk::PropertyFlag::Serializable>                                                     \
                ::Invoke(std::forward<Function>(func), std::forward<Args>(args)...);                                        \
        }                                                                                                                   \
                                                                                                                            \
        template<typename Function, typename ...Args>                                                                       \
        static decltype(auto) InvokeWithSerializables(Function&& func, Args&& ...args) {                                    \
            return FilteredWithTypesInvoker<::Quirk::PropertyFlag::Serializable>                                            \
                ::Invoke(std::forward<Function>(func), std::forward<Args>(args)...);                                        \
        }                                                                                                                   \
    };
    
    //_____________________________________________________________________________________________________________________________

}
