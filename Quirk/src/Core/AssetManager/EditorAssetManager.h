

#pragma once

#include "AssetImporter/AssetImporter.h"
#include "AssetManagerBase.h"

#include <unordered_map>


namespace Quirk {

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
			if (!IsAssetHandleValid(handle)) {
				QK_CORE_WARN("Requested Asset {0} do not exist in the registry", static_cast<uint64_t>(handle));
				return nullptr;
			}

			if (IsAssetLoaded(handle)) {
				return m_LoadedAssets[handle];
			}
			else {
				Ref<Asset> loadedAsset = AssetImporter::Import(m_AssetRegistry[handle]);
				m_LoadedAssets.emplace(handle, loadedAsset);
				return loadedAsset;
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
