

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
    // - think about this probable leak from raw pointer
    //

    template<ComplexReflectable T, typename FactoryFunc>
    static auto DeserializeReflectableImpl(const YAML::Node& node) {
        static_assert(!std::is_void_v<FactoryFunc>, "No Create or Factory for DeserializingType is Provided");

        using DeserializingType = PointingType_T<T>;

        if (!node) {
            throw std::runtime_error{ std::string{"YAML node is null for type "} + typeid(T).name() };
        }

        constexpr bool isPtrFactory = (
            std::is_same_v < FactoryFunc, typename Reflect<DeserializingType>::CreatePtrOrVoid_T   > ||
            std::is_same_v < FactoryFunc, typename Reflect<DeserializingType>::CreateRefOrVoid_T   > ||
            std::is_same_v < FactoryFunc, typename Reflect<DeserializingType>::CreateScopeOrVoid_T >
        );

        if constexpr (isPtrFactory) {
            return *( Reflect<DeserializingType>::InvokeWithSerializables(
                []<typename ...Properties> (const YAML::Node & node) {
                    return FactoryFunc::Invoke(
                        Deserializer<typename Properties::Type>::Deserialize(node[Properties::PropertyName])...
                    );
                }, 
                node
            ));
        }
        else {
            return Reflect<DeserializingType>::InvokeWithSerializables(
                []<typename ...Properties> (const YAML::Node & node) {
                    return FactoryFunc::Invoke(
                        Deserializer<typename Properties::Type>::Deserialize(node[Properties::PropertyName])...
                    );
                }, 
                node
            );
        }
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
            using factory = FirstNonVoid_T<
                Reflect<T>::CreateWithFactoryOrVoid_T,
                Reflect<T>::CreateOrVoid_T,
                Reflect<T>::CreatePtrOrVoid_T,
                Reflect<T>::CreateRefOrVoid_T,
                Reflect<T>::CreateScopeOrVoid_T
            >;

            return DeserializeReflectableImpl<T, factory>(node);
        }
    };

    template<Pointer T, typename FactoryFunc>
    static auto DeserializePtrImpl(const YAML::Node& node) {
        static_assert(!std::is_void_v<FactoryFunc>, "No factory Provided!");

        using DeserializingType = PointingType_T<T>;

        if constexpr (std::is_same_v<FactoryFunc, typename Reflect<DeserializingType>::CreateWithFactoryOrVoid_T>) {
            return ConvertPointer<T>(new T(DeserializeReflectableImpl<T, FactoryFunc>(node)));
        }
        if constexpr (std::is_same_v<FactoryFunc, typename Reflect<DeserializingType>::CreateOrVoid_T>) {
            return ConvertPointer<T>( Reflect<DeserializingType>::InvokeWithSerializables (
                [] <typename ...Properties> (const YAML::Node & node) {
                    return new DeserializingType(Deserializer<typename Properties::Type>::Deserialize(node[Properties::PropertyName])...);
                },
                node
            ));
        }
        else {
            return ConvertPointer<T>( Reflect<DeserializingType>::InvokeWithSerializables (
                [] <typename ...Properties> (const YAML::Node & node) {
                    return FactoryFunc::Invoke(
                        Deserializer<typename Properties::Type>::Deserialize(node[Properties::PropertyName])...
                    );
                },
                node
            ));
        }
    }

    template<ComplexReflectable T>
    struct Deserializer<T*> {
    public:
        static T* Deserialize(const YAML::Node& node) {
            using factory = FirstNonVoid_T<
                Reflect<T>::CreatePtrOrVoid_T,
                Reflect<T>::CreateRefOrVoid_T,
                Reflect<T>::CreateScopeOrVoid_T,
                Reflect<T>::CreateWithFactoryOrVoid_T,
                Reflect<T>::CreateOrVoid_T
            >;

            return DeserializePtrImpl<T*, factory>(node);
        }
    };

    template<ComplexReflectable T>
    struct Deserializer<Ref<T>> {
    public:
        static Ref<T> Deserialize(const YAML::Node& node) {
            using factory = FirstNonVoid_T<
                Reflect<T>::CreateRefOrVoid_T,
                Reflect<T>::CreatePtrOrVoid_T,
                Reflect<T>::CreateScopeOrVoid_T,
                Reflect<T>::CreateWithFactoryOrVoid_T,
                Reflect<T>::CreateOrVoid_T
            >;

            return DeserializePtrImpl<Ref<T>, factory>(node);
        }
    };

    template<ComplexReflectable T>
    struct Deserializer<Scope<T>> {
    public:
        static Scope<T> Deserialize(const YAML::Node& node) {
            using factory = FirstNonVoid_T<
                Reflect<T>::CreateScopeOrVoid_T,
                Reflect<T>::CreatePtrOrVoid_T,
                Reflect<T>::CreateRefOrVoid_T,
                Reflect<T>::CreateWithFactoryOrVoid_T,
                Reflect<T>::CreateOrVoid_T
            >;

            return DeserializePtrImpl<Scope<T>, factory>(node);
        }
    };

}
