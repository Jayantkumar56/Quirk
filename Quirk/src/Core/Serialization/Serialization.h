

#pragma once


#include "Serializer.h"
#include "PrimitiveSerializer.h"
#include "ContainerSerializer.h"

#include "Deserializer.h"
#include "PrimitiveDeserializer.h"
#include "ContainerDeserializer.h"


namespace Quirk {


    struct Serialization {
        static bool SerializeWithFieldName(std::string_view fieldName, const auto& data, const std::filesystem::path& path) {
            const auto parentPath = path.parent_path();

            if (!parentPath.empty() && !std::filesystem::exists(parentPath)) {
                QK_CORE_ERROR("Provided directory for file {0} doesn't exist!", path.string());
                return false;
            }
            if (!path.has_filename()) {
                QK_CORE_ERROR("Provided file path {0} doesn't contain a file name!", path.string());
                return false;
            }

            YAML::Emitter outEmitter;

            outEmitter << YAML::BeginMap;
            outEmitter << YAML::Key << fieldName;
            outEmitter << YAML::Value;

            // when serialization failed not saving the corrupted yaml data in file
            if (!Serializer<std::remove_cvref_t<decltype(data)>>::Serialize(data, outEmitter)) {
                return false;
            }

            outEmitter << YAML::EndMap;

            std::ofstream outFile(path);
            outFile << outEmitter.c_str();

            return true;
        }

        static bool Serialize(const auto& data, const std::filesystem::path& path) {
            using SerializingType = PointingType_T<decltype(data)>;

            if constexpr (HasReflection_V<SerializingType>) {
                return SerializeWithFieldName(Reflect<SerializingType>::TypeName, data, path);
            }
            else if constexpr (HasEnumReflection_V<SerializingType>) {
                return SerializeWithFieldName(EnumRegistry<SerializingType>::EnumName, data, path);
            }
            else {
                static_assert(AlwaysFalse_V<SerializingType>, "Unsupported type provided for Serialization!");
            }
        }

        template<typename T>
        static auto DeserializeWithFieldName(std::string_view fieldName, const std::filesystem::path& path) {
            const auto parentPath = path.parent_path();

            if (!parentPath.empty() && !std::filesystem::exists(parentPath)) {
                throw std::runtime_error{ 
                    std::string{"Provided directory for file "} + path.string() + std::string{" doesn't exist!"}
                };
            }
            if (!path.has_filename()) {
                throw std::runtime_error{
                    std::string{"Provided file path "} + path.string() + std::string{" doesn't contain a file name!"}
                };
            }

            YAML::Node node;

            try {
                node = YAML::LoadFile(path.string());
            }
            catch (const YAML::ParserException& e) {
                throw std::runtime_error{
                    std::string{"Failed to load file at "} + path.string() + std::string{" \n"} + std::string{e.what()}
                };
            }

            return Deserializer<T>::Deserialize(node[fieldName]);
        }

        template<typename T>
        static auto Deserialize(const std::filesystem::path& path) {
            using SerializingType = PointingType_T<T>;

            if constexpr (HasReflection_V<SerializingType>) {
                return DeserializeWithFieldName<T>(Reflect<SerializingType>::TypeName, path);
            }
            else if constexpr (HasEnumReflection_V<SerializingType>) {
                return DeserializeWithFieldName<T>(EnumRegistry<SerializingType>::EnumName, path);
            }
            else {
                static_assert(AlwaysFalse_V<SerializingType>, "Unsupported type provided for Serialization!");
            }
        }
    };

}
