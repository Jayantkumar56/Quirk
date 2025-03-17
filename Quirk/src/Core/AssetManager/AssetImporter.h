

#pragma once

#include "Core/Core.h"
#include "Asset.h"


namespace Quirk {

	struct AssetMetadata {
		AssetType Type = AssetType::None;
		std::filesystem::path Path;
	};

	class AssetImporter {
	public:
		static inline Ref<Asset> Import(const AssetMetadata& assetData) {
			switch (assetData.Type) {
				case AssetType::Texture2D : return Texture2DImporter (assetData);
				case AssetType::Scene     : return SceneImporter     (assetData);
			}

			QK_CORE_WARN("Invalid or unsupported AssetType \"{0}\" found!", static_cast<int>(assetData.Type));
			return nullptr;
		}

		static Ref<Asset> Texture2DImporter (const AssetMetadata& assetData);
		static Ref<Asset> SceneImporter     (const AssetMetadata& assetData);
	};

}
