

#pragma once

#include "Core/Utility/TypeTraits.h"

namespace Quirk {

    // Note:
    // 
    // - Must specialize for T to Serialize
    //   untill a proper general Serializer pipeline is created
    //

    template<typename T>
    struct Serializer {
        static_assert(AlwaysFalseV<T>, "Specialization not yet defined for Deserializer!");
    };

    // Note:
    // 
    // - Must specialize for T to Deserialize
    //   untill a proper general Deserializer pipeline is created
    //

    template<typename T>
    struct Deserializer {
        static_assert(AlwaysFalseV<T>, "Specialization not yet defined for Deserializer!");
    };

}
