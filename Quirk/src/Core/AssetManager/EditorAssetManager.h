

#pragma once

#include "AssetManagerBase.h"

#include <unordered_map>


namespace Quirk {

	struct AssetMetadata {
		AssetType Type;
		std::filesystem::path Path;
	};

	class EditorAssetManager : public AssetManagerBase {
	public:
		EditorAssetManager()          = default;
		virtual ~EditorAssetManager() = default;

		virtual inline AssetType GetAssetType(AssetHandle handle) override {
			if (!IsAssetHandleValid(handle))
				return AssetType::None;

			return m_AssetRegistry[handle].Type;
		}

		virtual Ref<Asset> GetAsset(AssetHandle handle) override {
			if (!IsAssetHandleValid(handle))
				return nullptr;

			if (IsAssetLoaded(handle)) {
				return m_LoadedAssets[handle];
			}
			else {

			}

			return nullptr;
		}

		virtual inline bool IsAssetHandleValid(AssetHandle handle) override {
			return m_AssetRegistry.contains(handle);
		}

		virtual inline bool IsAssetLoaded(AssetHandle handle) override {
			return m_LoadedAssets.contains(handle);
		}

	private:
		std::unordered_map<AssetHandle, Ref<Asset>>    m_LoadedAssets;
		std::unordered_map<AssetHandle, AssetMetadata> m_AssetRegistry;
	};

}
