

#pragma once

#include "Core/Utility/TypeTraits.h"
#include "AssetStorage.h"
#include "Core/AssetImporter/AssetImporter.h"
#include "Core/RHI/Factory.h"

#include <unordered_map>


namespace Quirk {

	class EditorAssetManager {
	public:
        EditorAssetManager() noexcept = default;

        EditorAssetManager(std::unordered_map<AssetHandle, std::filesystem::path> registry) noexcept :
                m_AssetRegistry(std::move(registry))
        {}

        // Create a default-constructed asset object in memory.
        template<typename T>
        AssetHandle CreateNewAsset(const std::filesystem::path& desiredPath) {
            Ref<T> newAsset = std::make_shared<T>();

            if (!AssetImporter<T>::Save(newAsset, desiredPath)) {
                return {}; // Return invalid handle
            }

            AssetHandle newHandle = RegisterAsset(desiredPath);
            m_AssetStorage.AddAsset<T>(newHandle, newAsset);

            return newHandle;
        }

        template<typename T>
        AssetHandle ImportNewAssetFromSource(const std::filesystem::path& sourcePath) {
            // Determine the path for the new serialized asset file (e.g., image.png -> image.texture_asset)
            std::filesystem::path assetPath = sourcePath;
            assetPath.replace_extension(AssetImporter<T>::GetAssetFileExtension());

            // TODO: Add a check here to see if the asset has already been imported to avoid duplicates.

            // Create a default asset object in memory from the source file.
            // This assumes the importer has a function to create a default asset.
            Ref<T> newAsset = AssetImporter<T>::CreateFromSource(sourcePath, m_RHIFactory);

            // Save this new asset's settings to disk for the first time.
            if (!AssetImporter<T>::Save(newAsset, assetPath)) {
                // Handle error: could not save the new asset file.
                return {}; // Return invalid handle
            }

            AssetHandle newHandle = RegisterAsset(assetPath);
            m_AssetStorage.AddAsset<T>(newHandle, newAsset);

            return newHandle;
        }

        AssetHandle RegisterAsset(const std::filesystem::path& assetPath) {
            AssetHandle handle; // Creates a new unique handle
            m_AssetRegistry[handle] = assetPath;
            return handle;
        }

        template<typename T>
        Ref<T> GetAsset(AssetHandle handle) {
            // If the asset is already loaded, return it from the type-safe storage.
            if (IsAssetLoaded<T>(handle)) {
                return m_AssetStorage.GetAsset<T>(handle);
            }

            // If not loaded, import it from its file path.
            if (!IsAssetHandleValid(handle)) {
                // Handle points to nothing on disk
                return nullptr;
            }

            const auto& path = m_AssetRegistry.at(handle);
            Ref<T> importedAsset = AssetImporter<T>::Import(path, m_RHIFactory);

            m_AssetStorage.AddAsset<T>(handle, importedAsset);
            return importedAsset;
        }

        template<typename T>
        bool SaveAsset(AssetHandle handle) {
            if (!IsAssetLoaded<T>(handle)) {
                // Cannot save an asset that isn't in memory
                return false;
            }

            Ref<T> asset = GetAsset<T>(handle);
            if (!asset->IsDirty()) {
                // No changes to save
                return true;
            }

            const auto& path = m_AssetRegistry.at(handle);
            if (AssetImporter<T>::Save(asset, path)) {
                asset->ClearDirtyFlag();
                return true;
            }

            return false;
        }

        inline bool IsAssetHandleValid(AssetHandle handle) const {
            return m_AssetRegistry.contains(handle);
        }

        template<typename T>
        bool IsAssetLoaded(AssetHandle handle) const {
            return m_AssetStorage.HasAsset<T>(handle);
        }

        inline const bool HasFactory() const noexcept { return m_RHIFactory.IsValid(); }

        inline void SetRHIFactory(ConstView<RHI::Factory> factory) noexcept {
            m_RHIFactory = factory;
        }

        inline const auto& GetAssetRegistry() const noexcept { return m_AssetRegistry; }

	private:
        ConstView<RHI::Factory> m_RHIFactory;
        AssetStorage            m_AssetStorage;
        std::unordered_map<AssetHandle, std::filesystem::path> m_AssetRegistry;
	};

}
