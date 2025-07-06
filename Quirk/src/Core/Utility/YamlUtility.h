

#pragma once

#include "glm/glm.hpp"
#include "yaml-cpp/yaml.h"

#include <string_view>
#include <vector>


namespace Quirk {

    template<typename T>
    T SafeRead(const YAML::Node& node, const std::string& key) {
        if (node[key])
            return node[key].as<T>();
        return T{};
    }

    template<typename T>
    T SafeRead(const YAML::Node& node, const std::string& key, const T& defaultValue) {
        if (node[key])
            return node[key].as<T>();
        return defaultValue;
    }

    template<typename T>
    T SafeRead(const YAML::Node& node, const char* key) {
        if (node[key])
            return node[key].as<T>();
        return T{};
    }

    template<typename T>
    T SafeRead(const YAML::Node& node, const char* key, const T& defaultValue) {
        if (node[key])
            return node[key].as<T>();
        return defaultValue;
    }

}


namespace YAML {

    //inline Emitter& operator << (Emitter& out, const std::string_view& str) {
    //    out << str.data();
    //    return out;
    //}

    inline Emitter& operator << (Emitter& out, const glm::vec2& v) {
        out << Flow;
        out << BeginSeq << v.x << v.y << EndSeq;
        return out;
    }

    template<>
    struct convert<glm::vec2> {
        static Node encode(const glm::vec2& vec) {
            Node node;
            node.push_back(vec.x);
            node.push_back(vec.y);
            return node;
        }

        static bool decode(const Node& node, glm::vec2& vec) {
            if (!node.IsSequence() || node.size() != 2)
                return false;

            vec[0] = node[0].as<float>();
            vec[1] = node[1].as<float>();

            return true;
        }
    };

    inline Emitter& operator << (Emitter& out, const glm::vec3& v) {
        out << Flow;
        out << BeginSeq << v.x << v.y << v.z << EndSeq;
        return out;
    }

    template<>
    struct convert<glm::vec3> {
        static Node encode(const glm::vec3& vec) {
            Node node;
            node.push_back(vec.x);
            node.push_back(vec.y);
            node.push_back(vec.z);
            return node;
        }

        static bool decode(const Node& node, glm::vec3& vec) {
            if (!node.IsSequence() || node.size() != 3)
                return false;

            vec[0] = node[0].as<float>();
            vec[1] = node[1].as<float>();
            vec[2] = node[2].as<float>();

            return true;
        }
    };

    inline Emitter& operator << (Emitter& out, const glm::vec4& v) {
        out << Flow;
        out << BeginSeq << v.x << v.y << v.z << v.w << EndSeq;
        return out;
    }

    template<>
    struct convert<glm::vec4> {
        static Node encode(const glm::vec4& vec) {
            Node node;
            node.push_back(vec.x);
            node.push_back(vec.y);
            node.push_back(vec.z);
            node.push_back(vec.w);
            return node;
        }

        static bool decode(const Node& node, glm::vec4& vec) {
            if (!node.IsSequence() || node.size() != 4)
                return false;

            vec[0] = node[0].as<float>();
            vec[1] = node[1].as<float>();
            vec[2] = node[2].as<float>();
            vec[3] = node[3].as<float>();

            return true;
        }
    };

    inline Emitter& operator<<(Emitter& out, const std::filesystem::path& p) {
        out << p.string();
        return out;
    }

    template<>
    struct convert<std::filesystem::path> {
        static Node encode(const std::filesystem::path& p) {
            Node node;
            node = p.string();
            return node;
        }

        static bool decode(const Node& node, std::filesystem::path& p) {
            if (!node.IsScalar())
                return false;

            p = node.as<std::string>();
            return true;
        }
    };

}
