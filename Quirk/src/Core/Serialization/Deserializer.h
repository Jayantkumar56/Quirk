

#pragma once


#include "Core/Core.h"
#include "Core/Utility/TypeTraits.h"
#include "Core/Reflection/Reflection.h"
#include "Core/Reflection/EnumRegistry.h"

#include "yaml-cpp/yaml.h"

#include <filesystem>


namespace Quirk {

    template<typename T>
    struct Deserializer {
        static_assert(AlwaysFalse_V<T>, "Unsupported type provided for Serialization");
    };


    // NOTE:
    //
    // - a probable leak may occur when a raw pointer of a property is created
    //   and an exception is thrown in subsequent property construction
    // 
    // TODO:
    // 
    // - think about this probable peak from raw pointer
    //

    template<ComplexReflectable T, typename ...FactoryFuncs>
    static T DeserializeReflectableImpl(const YAML::Node& node) {
        using DeserializingType      = std::remove_cvref_t<std::conditional_t<IsPointer_V<T>, PointingType_T<T>, T>>;
        using PrioritisedFactoryFunc = FirstNonVoid_T<FactoryFuncs...>;

        // done this way to get DeserializingType in the compiler error message when it fails
        if constexpr (std::is_void_v<PrioritisedFactoryFunc>) {
            static_assert(AlwaysFalse_V<DeserializingType>, "No Create or Factory for DeserializingType is Provided");
        }

        if (!node) {
            throw std::runtime_error{ std::string{"YAML node is null for type "} + typeid(T).name() };
        }

        return Reflect<DeserializingType>::InvokeWithSerializables(
            []<typename ...Properties> (const YAML::Node & node) -> T {
                return PrioritisedFactoryFunc::Invoke(
                    Deserializer<typename Properties::Type>::Deserialize(node[Properties::PropertyName])...
                );
            }, 
            node
        );
    }


    template<EnumReflectable T>
    struct Deserializer<T> {
    public:
        static T Deserialize(const YAML::Node& node) {
            if (!node) {
                throw std::runtime_error{ std::string{"YAML node is null for type "} + typeid(T).name() };
            }

            return EnumRegistry<T>::FromString(node.as<std::string>());
        }
    };

    template<ComplexReflectable T>
    struct Deserializer<T> {
    public:
        static T Deserialize(const YAML::Node& node) {
            return DeserializeReflectableImpl<T, 
                Reflect<T>::CreateWithFactoryOrVoid_T, 
                Reflect<T>::CreateOrVoid_T 
            >(node);
        }
    };

    template<ComplexReflectable T>
    struct Deserializer<T*> {
    public:
        static T* Deserialize(const YAML::Node& node) {
            auto data = DeserializeReflectableImpl < T*,
                Reflect<T>::CreatePtrOrVoid_T,
                Reflect<T>::CreateRefOrVoid_T,
                Reflect<T>::CreateScopeOrVoid_T
            >(node);

            if constexpr (IsRawPointer_V<decltype(data)>) {
                return data;
            }
            else {
                static_assert(IsSmartPointer_V<decltype(data)>, "Unexpected factory return type");
                return data.get();
            }
        }
    };

    template<ComplexReflectable T>
    struct Deserializer<Ref<T>> {
    public:
        static Ref<T> Deserialize(const YAML::Node& node) {
            auto data = DeserializeReflectableImpl < Ref<T>,
                Reflect<T>::CreateRefOrVoid_T,
                Reflect<T>::CreatePtrOrVoid_T,
                Reflect<T>::CreateScopeOrVoid_T
            >(node);

            if constexpr (std::is_same_v<decltype(data), Ref<T>>) {
                return data;
            }
            else if constexpr (IsRawPointer_V<decltype(data)>) {
                return Ref<T>(data);
            }
            else {
                static_assert(std::is_same_v<decltype(data), Scope<T>>, "Unexpected factory return type");
                return Ref<T>(data.get());
            }
        }
    };

    template<ComplexReflectable T>
    struct Deserializer<Scope<T>> {
    public:
        static Scope<T> Deserialize(const YAML::Node& node) {
            auto data = DeserializeReflectableImpl < Scope<T>,
                Reflect<T>::CreateScopeOrVoid_T,
                Reflect<T>::CreatePtrOrVoid_T,
                Reflect<T>::CreateRefOrVoid_T
            >(node);

            if constexpr (std::is_same_v<decltype(data), Scope<T>>) {
                return data;
            }
            else if constexpr (IsRawPointer_V<decltype(data)>) {
                return Scope<T>(data);
            }
            else {
                static_assert(std::is_same_v<decltype(data), Ref<T>>, "Unexpected factory return type");
                return Scope<T>(data.get());
            }
        }
    };

}
