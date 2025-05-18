

#pragma once

#include "Core/Core.h"
#include "Core/Utility/TypeTraits.h"
#include "Core/Reflection/Reflection.h"
#include "Core/Reflection/EnumRegistry.h"

#include "yaml-cpp/yaml.h"

#include <filesystem>

namespace Quirk {

    template<typename T>
    struct Serializer {
        static_assert(AlwaysFalse_V<T>, "Unsupported type provided for Serialization!");
    };

    template<EnumReflectable T>
    struct Serializer<T> {
        [[nodiscard("Must check the result of Serializer::Serialize or errors may go unnoticed")]]
        static bool Serialize(const T data, YAML::Emitter& outEmitter) {
            if (!outEmitter.good()) {
                QK_CORE_ERROR("Provided emitter for Type {0} is corrupted!", typeid(T).name());
                return false;
            }

            outEmitter << EnumRegistry<T>::ToString(data);
            return true;
        }
    };

    template<ComplexReflectable T>
    struct Serializer<T> {
    public:
        [[nodiscard("Must check the result of Serializer::Serialize or errors may go unnoticed")]]
        static bool Serialize(const T& data, YAML::Emitter& outEmitter) {
            if (!outEmitter.good()) {
                QK_CORE_ERROR("Provided emitter for Type {0} is corrupted!", typeid(T).name());
                return false;
            }

            outEmitter << YAML::BeginMap;

            bool isOk = true;

            Reflect<T>::ForEachSerializable(
                [] <typename Property> (const T& data, YAML::Emitter& outEmitter, bool& isOk) -> bool {
                    if (!isOk)
                        return false;

                    outEmitter << YAML::Key   << Property::PropertyName;
                    outEmitter << YAML::Value;

                    isOk = Serializer<typename Property::Type>::Serialize(Property::Get(data), outEmitter);
                    return isOk;
                },
                data,
                outEmitter,
                isOk
            );

            outEmitter << YAML::EndMap;
            return isOk;
        }
    };

    template<Pointer T>
    struct Serializer<T> {
        [[nodiscard("Must check the result of Serializer::Serialize or errors may go unnoticed")]]
        static bool Serialize(const T& data, YAML::Emitter& outEmitter) {
            using SerializingType = std::remove_cvref_t<decltype(*data)>;

            if (!data) {
                QK_CORE_ERROR("NULL pointer of type {0} is provided for serialization!", typeid(SerializingType).name());
                return false;
            }

            return Serializer<SerializingType>::Serialize(*data, outEmitter);
        }
    };

}
