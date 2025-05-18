
#pragma once

#include "Core/Utility/YamlUtility.h"
#include "Serializer.h"

#include <vector>
#include <unordered_map>

namespace Quirk {

    template<typename T>
    struct Serializer<std::vector<T>> {
        static inline bool Serialize(const std::vector<T>& vec, YAML::Emitter& outEmitter) {
            if (!outEmitter.good()) {
                QK_CORE_WARN("Emitter is corrupted while serializing type: std::vector of {0}", typeid(T).name());
                return false;
            }

            outEmitter << YAML::BeginSeq;

            for (const auto& data : vec) {
                if (!Serializer<T>::Serialize(data, outEmitter)) {
                    QK_CORE_WARN(
                        "Error occured while deserializing: std::vector of {0}, {1}",
                        typeid(T).name()
                    );
                    return false;
                }
            }

            outEmitter << YAML::EndSeq;

            return true;
        }
    };

    template<typename Key, typename Value>
    struct Serializer<std::unordered_map<Key, Value>> {
        static inline bool Serialize(const std::unordered_map<Key, Value>& map, YAML::Emitter& outEmitter) {
            if (!outEmitter.good()) {
                QK_CORE_WARN(
                    "Emitter is corrupted while serializing type: std::unordered_map of {0}, {1}", 
                    typeid(Key).name(), 
                    typeid(Value).name()
                );
                return false;
            }

            outEmitter << YAML::BeginMap;

            for (const auto& [key, value] : map) {
                outEmitter << YAML::Key;
                if (!Serializer<Key>::Serialize(key, outEmitter)) {
                    QK_CORE_WARN(
                        "Error occured while deserializing key of: std::unordered_map of {0}, {1}",
                        typeid(Key).name(),
                        typeid(Value).name()
                    );
                    return false;
                }

                outEmitter << YAML::Value;
                if (!Serializer<Value>::Serialize(value, outEmitter)) {
                    QK_CORE_WARN(
                        "Error occured while deserializing value of: std::unordered_map of {0}, {1}",
                        typeid(Key).name(),
                        typeid(Value).name()
                    );
                    return false;
                }
            }

            outEmitter << YAML::EndMap;

            return true;
        }
    };

}
