

#pragma once

#include "Core/Core.h"
#include "Core/Assets/Asset.h"


#include "TextureImporter.h"


namespace Quirk {

	struct AssetMetadata {
		AssetType Type = AssetType::None;
		std::filesystem::path Path;
	};

	class AssetImporter {
	public:
        static inline Ref<Asset> Import(const AssetMetadata& assetData) {
            switch (assetData.Type) {
                case AssetType::Texture2D: return Texture2DImporter::ImportFromMetadata(assetData.Path);
            }

            QK_CORE_WARN("Invalid or unsupported AssetType \"{0}\" found!", static_cast<int>(assetData.Type));
            return nullptr;
        }

        static inline Ref<Asset> Import(const AssetHandle handle, const AssetMetadata& assetData) {
            Ref<Asset> asset;

            switch (assetData.Type) {
                case AssetType::Texture2D: {
                    asset = Texture2DImporter::ImportFromMetadata(assetData.Path); 
                    break;
                }

                default: {
                    QK_CORE_WARN("Invalid or unsupported AssetType \"{0}\" found!", static_cast<int>(assetData.Type)); 
                    return asset;
                }
            }

            if (asset != nullptr)
                asset->SetHandle(handle);

            return asset;
        }
	};

}
