

#pragma once

#include "Core/Core.h"
#include "Asset.h"


namespace Quirk {

	// Base class for both Runtime and Editor AssetManager
	class AssetManagerBase {
	public:
		AssetManagerBase()          = default;
		virtual ~AssetManagerBase() = default;

		virtual inline AssetType GetAssetType       (AssetHandle handle) = 0;
		virtual Ref<Asset>       GetAsset           (AssetHandle handle) = 0;
		virtual inline bool      IsAssetHandleValid (AssetHandle handle) = 0;
		virtual inline bool      IsAssetLoaded      (AssetHandle handle) = 0;
	};

}
