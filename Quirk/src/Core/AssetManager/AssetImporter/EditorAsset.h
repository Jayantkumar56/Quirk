

#pragma once

#include "Core/Core.h"
#include "Core/Utility/TypeTraits.h"


namespace Quirk {

    template<typename T>
    struct EditorAssetSpec { static_assert(AlwaysFalse_V<T>, "No EditorAssetSpec exist for given Asset type!"); };

    template<typename T>
    struct EditorAssetMeta { static_assert(AlwaysFalse_V<T>, "No EditorAssetMeta exist for given Asset type!"); };


    template <typename T>
    struct EditorAsset {
        Ref<T>             Asset;
        EditorAssetMeta<T> MetaData;

        inline Ref<T> GetAsset() const noexcept { return Asset;            }
        inline bool   IsLoaded() const noexcept { return Asset != nullptr; }
    };

}
