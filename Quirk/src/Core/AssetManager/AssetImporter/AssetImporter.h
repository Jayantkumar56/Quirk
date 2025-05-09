

#pragma once

#include "Core/Core.h"
#include "Core/Assets/Asset.h"
#include "Core/Utility/TypeTraits.h"

#include <filesystem>


namespace Quirk {

    // NOTE:
    //
    // - every EditorAsset specialization must have the following functions signatures:
    // 
    //   inline Ref<T> GetAsset() noexcept { }
    //   inline bool   IsLoaded() noexcept { }
    // 

    template <typename T>
    struct EditorAsset {
        Ref<T> Asset;
        std::filesystem::path AssetPath;

        inline Ref<T> GetAsset() noexcept { return Asset;            }
        inline bool   IsLoaded() noexcept { return Asset != nullptr; }

        static_assert(AlwaysFalseV<T>, "No Specialization created for AssetType T, must have specialization to be used");
    };

    // NOTE:
    //
    // - every AssetImporter specialization must have the following signature and must define:
    // 
    //   static AssetHandle Create(EditorAsset<T>& outAsset)  { }
    //   static void        Import(EditorAsset<T>& outAsset)  { }
    //   static void        Save(const EditorAsset<T>& asset) { }
    //

    template<typename T>
    class AssetImporter {
    public:
        static AssetHandle Create(EditorAsset<T>& outAsset)  { }
        static void        Import(EditorAsset<T>& outAsset)  { }
        static void        Save(const EditorAsset<T>& asset) { }

        static_assert(AlwaysFalseV<T>, "No importer exist for given Asset type!");
    };

}
