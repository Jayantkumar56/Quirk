

#pragma once

#include "Core/Utility/YamlUtility.h"
#include "Serializer.h"
#include "Core/Utility/UUID.h"


namespace Quirk {

#define DEFINE_PRIMITIVE_SERIALIZER_NON_REF(Type)                                                         \
    template<>                                                                                            \
    struct Serializer<Type> {                                                                             \
        static inline bool Serialize(const Type data, YAML::Emitter& outEmitter) {                        \
            if (!outEmitter.good()) {                                                                     \
                QK_CORE_WARN("Emitter is corrupted while serializing type: {0}", typeid(Type).name());    \
                return false;                                                                             \
            }                                                                                             \
                                                                                                          \
            outEmitter << data;                                                                           \
            return true;                                                                                  \
        }                                                                                                 \
    };

#define DEFINE_PRIMITIVE_SERIALIZER_REF(Type)                                                             \
    template<>                                                                                            \
    struct Serializer<Type> {                                                                             \
        static inline bool Serialize(const Type& data, YAML::Emitter& outEmitter) {                       \
            if (!outEmitter.good()) {                                                                     \
                QK_CORE_WARN("Emitter is corrupted while serializing type: {0}", typeid(Type).name());    \
                return false;                                                                             \
            }                                                                                             \
                                                                                                          \
            outEmitter << data;                                                                           \
            return true;                                                                                  \
        }                                                                                                 \
    };

    // integer types
    DEFINE_PRIMITIVE_SERIALIZER_NON_REF( int8_t   )
    DEFINE_PRIMITIVE_SERIALIZER_NON_REF( int16_t  )
    DEFINE_PRIMITIVE_SERIALIZER_NON_REF( int32_t  )
    DEFINE_PRIMITIVE_SERIALIZER_NON_REF( int64_t  )

    DEFINE_PRIMITIVE_SERIALIZER_NON_REF( uint8_t  )
    DEFINE_PRIMITIVE_SERIALIZER_NON_REF( uint16_t )
    DEFINE_PRIMITIVE_SERIALIZER_NON_REF( uint32_t )
    DEFINE_PRIMITIVE_SERIALIZER_NON_REF( uint64_t )

    DEFINE_PRIMITIVE_SERIALIZER_NON_REF( float  )
    DEFINE_PRIMITIVE_SERIALIZER_NON_REF( double )
    DEFINE_PRIMITIVE_SERIALIZER_NON_REF( bool   )

    // math types
    DEFINE_PRIMITIVE_SERIALIZER_REF( glm::vec2 )
    DEFINE_PRIMITIVE_SERIALIZER_REF( glm::vec3 )
    DEFINE_PRIMITIVE_SERIALIZER_REF( glm::vec4 )

    DEFINE_PRIMITIVE_SERIALIZER_REF( std::string )
    DEFINE_PRIMITIVE_SERIALIZER_REF( std::filesystem::path )


    template<>
    struct Serializer<UUID> {
        static inline bool Serialize(const UUID data, YAML::Emitter& outEmitter) {
            if (!outEmitter.good()) {
                QK_CORE_WARN("Emitter is corrupted while serializing type: UUID");
                return false;
            }
            
            outEmitter << static_cast<uint64_t>(data);
            return true;
        }
    };

}

