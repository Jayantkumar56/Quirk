

#pragma once

#include "EditorAsset.h"


namespace Quirk {

    // NOTE:
    //
    // - every AssetImporter specialization must have the following function signature and must define:
    // 
    //   static EditorAsset<T> Create(EditorAssetSpec<T> spec)
    //   static EditorAsset<T> Import(std::filesystem::path assetPath)
    //   static bool           Save(const EditorAsset<T>& asset)
    //

    template<typename T>
    class AssetImporter {
    public:
        static EditorAsset<T> Create(EditorAssetSpec<T> spec)         {}
        static EditorAsset<T> Import(std::filesystem::path assetPath) {}
        static bool           Save(const EditorAsset<T>& asset)       {}

        static_assert(AlwaysFalse_V<T>, "No importer exist for given Asset type!");
    };

}
 