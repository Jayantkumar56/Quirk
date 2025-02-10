

#pragma once

#include "Core/Utility/UUID.h"
#include "unordered_map"
#include "filesystem"

namespace Quirk {

	using AssetHandle = UUID;

	enum class AssetType {
		None,
		Texture2D,
		Scene
	};

	class Asset {
	public:
		virtual AssetType GetAssetType() = 0;

	private:
		AssetHandle m_Handle;
	};

}
