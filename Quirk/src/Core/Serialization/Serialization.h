

#pragma once


#include "Serializer.h"
#include "PrimitiveSerializer.h"
#include "ContainerSerializer.h"

#include "Deserializer.h"
#include "PrimitiveDeserializer.h"
#include "ContainerDeserializer.h"


namespace Quirk {


    struct Serialization {
        static bool Serialize(const auto& data, const std::filesystem::path& path) {
            using DataType        = std::remove_cvref_t<decltype(data)>;
            using SerializingType = std::conditional_t<IsPointer_V<DataType>, std::remove_cvref_t<decltype(*data)>, DataType>;

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

            if constexpr (HasReflection_V<SerializingType>) {
                outEmitter << YAML::Key << Reflect<SerializingType>::TypeName; 
            }
            else if constexpr (HasEnumReflection_V<SerializingType>) {
                outEmitter << YAML::Key << EnumRegistry<SerializingType>::EnumName;
            }
            else {
                static_assert(AlwaysFalse_V<SerializingType>, "Unsupported type provided for Serialization!");
            }

            outEmitter << YAML::Value;

            // when serialization failed not saving the corrupted yaml data in file
            if (!Serializer<DataType>::Serialize(data, outEmitter)) {
                return false;
            }

            outEmitter << YAML::EndMap;

            std::ofstream outFile(path);
            outFile << outEmitter.c_str();

            return true;
        }

        template<typename T>
        static auto Deserialize(const std::filesystem::path& path) {
            using SerializingType = std::conditional_t<IsPointer_V<T>, std::remove_cvref_t<PointingType_T<T>>, std::remove_cvref_t<T>>;

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

            if constexpr (HasReflection_V<SerializingType>) {
                return Deserializer<T>::Deserialize(node[Reflect<SerializingType>::TypeName]);
            }
            else if constexpr (HasEnumReflection_V<SerializingType>) {
                return Deserializer<T>::Deserialize(node[EnumRegistry<SerializingType>::EnumName]);
            }
            else {
                static_assert(AlwaysFalse_V<SerializingType>, "Unsupported type provided for Serialization!");
            }
        }
    };

}
