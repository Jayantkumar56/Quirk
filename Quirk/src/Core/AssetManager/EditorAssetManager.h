

#pragma once

#include "Core/Utility/TypeTraits.h"
#include "AssetImporter/TextureImporter.h"

#include <unordered_map>


namespace Quirk {

	class EditorAssetManager {
	public:
        ~EditorAssetManager() noexcept {
            try {
                // saving logic
                SaveAssets<Texture2D>();
            }
            catch (...) {
                QK_CORE_ERROR("Error occurred while saving Assets");
            }
        }

        template<typename T>
        AssetHandle RegisterAsset(EditorAsset<T>&& metaData) {
            AssetHandle handle = AssetImporter<T>::Create(metaData);

            GetStorage<T>().emplace(handle, std::move(metaData));
            return handle;
        }

        template<typename T>
        Ref<T> GetAsset(AssetHandle handle) {
            if (!IsAssetHandleValid<T>(handle)) {
                QK_CORE_WARN("Requested Asset {0} do not exist in the registry", static_cast<uint64_t>(handle));
                return nullptr;
            }

            EditorAsset<T>& asset = GetStorage<T>().at(handle);

            if (!asset->IsLoaded())
                AssetImporter<T>::Import(asset);

            return asset.GetAsset();
        }

        template<typename T>
		bool IsAssetHandleValid(AssetHandle handle) { return GetStorage<T>().contains(handle); }

        template<typename T>
		bool IsAssetLoaded(AssetHandle handle) { return GetStorage<T>().at(handle).IsLoaded(); }

    private:
        template<typename T>
        void SaveAssets() {
            const auto& storage = GetStorage<T>();

            for (const auto& asset : storage) {
                if (asset.second.IsLoaded())
                    AssetImporter<T>::Save(asset.second);
            }
        }

    private:
        // ---------------------------------------------------------------------------------------------------------------------
        // Storage per type access

        template<typename T>
        auto& GetStorage() { static_assert(AlwaysFalse_V<T>, "Storage is not defined for the Type!"); }

        template<> inline auto& GetStorage<Texture2D>() { return m_Texture2DStorage; }

	private:
		std::unordered_map<AssetHandle, EditorAsset<Texture2D>> m_Texture2DStorage;
	};

}
