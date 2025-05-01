

#pragma once

#include <type_traits>

namespace Quirk {

    template<typename T>
    struct AlwaysFalse : std::false_type {};

    template<typename T>
    inline constexpr bool AlwaysFalseV = AlwaysFalse<T>::value;

    template<typename... Types>
    struct TypeList {
        template<typename Function, typename ...Args>
        static void ForEach(Function&& func, Args&& ...args) {
            (func.template operator() < Types > (std::forward<Args>(args)...), ...);
        }
    };

}

