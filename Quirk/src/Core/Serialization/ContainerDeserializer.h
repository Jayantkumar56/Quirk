
#pragma once



#include "Core/Utility/YamlUtility.h"
#include "Deserializer.h"

#include <vector>
#include <unordered_map>

namespace Quirk {

    template<typename T>
    struct Deserializer<std::vector<T>> {
        static inline std::vector<T> Deserialize(const YAML::Node& node) {
            if (!node && !node.IsSequence()) {
                throw std::runtime_error{ std::string{"YAML node is null or is not sequence for type "} + typeid(T).name() };
            }

            std::vector<T> outVec;
            outVec.reserve(node.size());

            for (const auto& item : node) {
                outVec.emplace_back(Deserializer<T>::Deserialize(item));
            }

            return outVec;
        }
    };

    template<typename Key, typename Value>
    struct Deserializer<std::unordered_map<Key, Value>> {
        static inline std::unordered_map<Key, Value> Deserialize(const YAML::Node& node) {
            if (!node && !node.IsMap()) {
                throw std::runtime_error{ 
                    std::string{"YAML node is null or is not Map for Key = "} + typeid(Key).name() +
                    std::string{", Value = "} + typeid(Value).name()
                };
            }

            std::unordered_map<Key, Value> outMap;
            outMap.reserve(node.size());

            for (const auto& pair : node) {
                outMap.emplace(Deserializer<Key>::Deserialize(pair.first), Deserializer<Value>::Deserialize(pair.second));
            }

            return outMap;
        }
    };

}

