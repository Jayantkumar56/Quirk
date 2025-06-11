

#pragma once

#include "Core/Utility/TypeTraits.h"
#include "AssetImporter/TextureImporter.h"
#include "Core/RHI/Factory.h"

#include <unordered_map>


namespace Quirk {

    // NOTE:
    //
    // - this AssetManager works on the principle that
    //   "The caller knows what type of data they're working with."
    //

	class EditorAssetManager {
	public:
        EditorAssetManager() noexcept = default;

        EditorAssetManager(std::unordered_map<AssetHandle, std::filesystem::path> registry) noexcept :
                m_AssetRegistry(std::move(registry))
        {}

        template<typename T>
        AssetHandle RegisterAsset(EditorAsset<T>&& metaData) {
            // TODO: Register the asset 

            return AssetHandle();
        }

        template<typename T>
        Ref<T> GetAsset(AssetHandle handle) {
            if (!IsAssetHandleValid<T>(handle)) {
                QK_CORE_WARN("Requested Asset {0} do not exist in the registry", static_cast<uint64_t>(handle));
                return nullptr;
            }

           // TODO: load the asset if not loaded

            return nullptr;
        }

        template<typename T>
		bool IsAssetHandleValid(AssetHandle handle) { return GetStorage<T>().contains(handle); }

        template<typename T>
		bool IsAssetLoaded(AssetHandle handle) { return GetStorage<T>().at(handle).IsLoaded(); }

        inline const auto& GetAssetRegistry() const noexcept { return m_AssetRegistry;        }
        inline const bool  HasFactory()       const noexcept { return m_RHIFactory.IsValid(); }

        inline void SetRHIFactory(ConstView<RHI::Factory> factory) noexcept {
            m_RHIFactory = factory;
        }

    private:
        // === Begin: Per type Storage Access ===

        template<typename T>
        auto& GetStorage() { static_assert(AlwaysFalse_V<T>, "Storage is not defined for the Type!"); }

        template<> inline auto& GetStorage<RHI::Texture2D>() { return m_Texture2DStorage; }

        // === End:   Per type Storage Access ===

	private:
        ConstView<RHI::Factory> m_RHIFactory;

        std::unordered_map<AssetHandle, EditorAsset<RHI::Texture2D>> m_Texture2DStorage;

        std::unordered_map<AssetHandle, std::filesystem::path> m_AssetRegistry;
	};

}
