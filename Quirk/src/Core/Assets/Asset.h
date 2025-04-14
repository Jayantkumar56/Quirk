

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
        friend class AssetImporter;

	public:
		inline virtual AssetType GetAssetType() const noexcept = 0;

        inline AssetHandle GetHandle() const noexcept { return m_Handle; }

    private:
        inline void SetHandle(AssetHandle handle) noexcept { m_Handle = handle; }

	private:
		AssetHandle m_Handle;
	};

}
