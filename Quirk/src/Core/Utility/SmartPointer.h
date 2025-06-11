

#pragma once

#include <memory>


namespace Quirk {

    enum class Ownership {
        Raw, Ref, Scope
    };



    template<typename T>
    using Ref = std::shared_ptr<T>;

    template<typename T>
    using Scope = std::unique_ptr<T>;



    template<typename T, typename ...Args>
    Ref<T> CreateRef(Args&& ...args) {
        return std::make_shared<T>(std::forward<Args>(args)...);
    }

    template<typename T, typename ...Args>
    Scope<T> CreateScope(Args&& ...args) {
        return std::make_unique<T>(std::forward<Args>(args)...);
    }



    template<typename T, Ownership OwnerType, typename ...Args>
    auto CreatePtr(Args&& ...args) {
        if constexpr (OwnerType == Ownership::Raw) {
            return new T(std::forward<Args>(args)...);
        }
        else if constexpr (OwnerType == Ownership::Ref) {
            return CreateRef(std::forward<Args>(args)...);
        }
        else if constexpr (OwnerType == Ownership::Scope) {
            return CreateScope(std::forward<Args>(args)...);
        }
        else {
            static_assert(
                OwnerType == Ownership::Raw ||
                OwnerType == Ownership::Ref ||
                OwnerType == Ownership::Scope,
                "Invalid ownership type"
            );
        }
    }

}

