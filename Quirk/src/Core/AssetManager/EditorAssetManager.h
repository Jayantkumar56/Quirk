

#pragma once

#include "Core/Utility/TypeTraits.h"
#include "AssetImporter/TextureImporter.h"

#include <unordered_map>


namespace Quirk {

	class EditorAssetManager {
	public:
        template<typename T>
        inline AssetHandle RegisterAsset(AssetMetadata metaData) {
            Ref<T>  loadedAsset = AssetImporter<T>::Import(metaData);    
            AssetHandle handle  = loadedAsset->GetHandle();

            GetStorage<T>().emplace(handle, loadedAsset);
            GetRegistry<T>().emplace(handle, std::move(metaData));

            return handle;
        }

        template<typename T>
        Ref<T> GetAsset(AssetHandle handle) {
            if (!IsAssetHandleValid<T>(handle)) {
                QK_CORE_WARN("Requested Asset {0} do not exist in the registry", static_cast<uint64_t>(handle));
                return nullptr;
            }

            auto& assetStorage = GetStorage<T>();

            if (auto asset = assetStorage.find(handle); asset != assetStorage.end()) {
                return asset->second;
            }
            else {
                Ref<T> loadedAsset = AssetImporter<T>::Import(handle, GetRegistry<T>().at(handle));
                assetStorage.emplace(handle, loadedAsset);
                return loadedAsset;
            }

            return nullptr;
        }

        template<typename T>
		inline bool IsAssetHandleValid(AssetHandle handle) { return GetRegistry<T>().contains(handle); }

        template<typename T>
		bool IsAssetLoaded(AssetHandle handle) { return GetStorage<T>().contains(handle); }

    private:
        // ---------------------------------------------------------------------------------------------------------------------
        // Storage per type access

        template<typename T>
        auto& GetStorage() { static_assert(AlwaysFalseV<T>, "Storage is not defined for the Type!"); }

        template<> inline auto& GetStorage<Texture2D>() { return m_Texture2DStorage; }

        // ---------------------------------------------------------------------------------------------------------------------
        // Registry per type access

        template<typename T>
        auto& GetRegistry() { static_assert(AlwaysFalseV<T>, "Registry is not defined for the Type!"); }

        template<> inline auto& GetRegistry<Texture2D>() { return m_Texture2DRegistry; }

	private:
        std::unordered_map<AssetHandle, Ref<Texture2D>> m_Texture2DStorage;
		std::unordered_map<AssetHandle, AssetMetadata> m_Texture2DRegistry;
	};

}
