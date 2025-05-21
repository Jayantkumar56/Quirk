

#pragma once

#include "Core/Utility/YamlUtility.h"
#include "Deserializer.h"
#include "Core/Utility/UUID.h"

namespace Quirk {

#define DEFINE_PRIMITIVE_DESERIALIZER(Type)                                                                     \
    template<>                                                                                                  \
    struct Deserializer<Type> {                                                                                 \
        static inline Type Deserialize(const YAML::Node& node) {                                                \
            if (!node) {                                                                                        \
                throw std::runtime_error{ std::string{"YAML node is null for type "} + typeid(Type).name() };   \
            }                                                                                                   \
                                                                                                                \
            return node.as<Type>();                                                                             \
        }                                                                                                       \
    };

    // integer types
    DEFINE_PRIMITIVE_DESERIALIZER( int8_t  )
    DEFINE_PRIMITIVE_DESERIALIZER( int16_t )
    DEFINE_PRIMITIVE_DESERIALIZER( int32_t )
    DEFINE_PRIMITIVE_DESERIALIZER( int64_t )

    DEFINE_PRIMITIVE_DESERIALIZER( uint8_t  )
    DEFINE_PRIMITIVE_DESERIALIZER( uint16_t )
    DEFINE_PRIMITIVE_DESERIALIZER( uint32_t )
    DEFINE_PRIMITIVE_DESERIALIZER( uint64_t )

    DEFINE_PRIMITIVE_DESERIALIZER( float  )
    DEFINE_PRIMITIVE_DESERIALIZER( double )
    DEFINE_PRIMITIVE_DESERIALIZER( bool   )

    // math types
    DEFINE_PRIMITIVE_DESERIALIZER( glm::vec2 )
    DEFINE_PRIMITIVE_DESERIALIZER( glm::vec3 )
    DEFINE_PRIMITIVE_DESERIALIZER( glm::vec4 )

    DEFINE_PRIMITIVE_DESERIALIZER( std::string )
    DEFINE_PRIMITIVE_DESERIALIZER( std::filesystem::path )


    template<>
    struct Deserializer<UUID> {
        static inline UUID Deserialize(const YAML::Node& node) {
            if (!node) {
                throw std::runtime_error{ std::string{"YAML node is null for type UUID"} };
            }
            
            return UUID{ node.as<uint64_t>() };
        }
    };

}
