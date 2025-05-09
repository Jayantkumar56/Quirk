

#pragma once

#include "glm/glm.hpp"
#include "yaml-cpp/yaml.h"

#include <string_view>


namespace YAML {

    static Emitter& operator << (Emitter& out, const std::string_view& str) {
        out << str.data();
        return out;
    }

    static Emitter& operator << (Emitter& out, const glm::vec2& v) {
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

    static Emitter& operator << (Emitter& out, const glm::vec3& v) {
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

    static Emitter& operator << (Emitter& out, const glm::vec4& v) {
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

}
