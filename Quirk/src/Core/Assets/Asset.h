

#pragma once

#include "Core/Utility/UUID.h"

namespace Quirk {

	using AssetHandle = UUID;

	class Asset {
        template<typename T>
        friend class AssetImporterBase;

	public:
        inline AssetHandle GetHandle() const noexcept { return m_Handle; }

    private:
        inline void SetHandle(AssetHandle handle) noexcept { m_Handle = handle; }

	private:
		AssetHandle m_Handle;
	};

}
