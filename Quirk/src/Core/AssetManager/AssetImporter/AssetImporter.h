

#pragma once

#include "Core/Core.h"
#include "Core/Assets/Asset.h"
#include "Core/Utility/TypeTraits.h"

#include <filesystem>


namespace Quirk {

	struct AssetMetadata {
		std::filesystem::path Path;
	};

    // base class template for all of the AssetImporter specializations
    // 
    // - can create assets with asset handle
    //   (used to load pre registered assets)

    template<typename Derived>
    class AssetImporterBase {
    public:
        static auto Import(const AssetHandle handle, const AssetMetadata& assetMeta) {
            auto asset = Derived::Import(assetMeta);
            asset->SetHandle(handle);
            return asset;
        }
    };

    // NOTE:
    //
    // - every AssetImporter specialization must have the following signature and must define:
    // 
    //    static Ref<AssetType> Import(const AssetMetadata&);
    // 
    // - inheriting from the AssetImporterBase with crtp would provide common 
    //   features to the AssetImporter specializations
    //

    template<typename T>
    class AssetImporter : public AssetImporterBase<AssetImporter<T>> {
    public:
        static Ref<T> Import(const AssetMetadata& assetMeta) {
            static_assert(AlwaysFalseV<T>, "No importer exist for given Asset type!");
        }
    };

}
