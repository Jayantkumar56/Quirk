

#pragma once

#include "Core/Core.h"
#include "Core/Assets/RenderAssets/Texture.h"
#include "AssetImporter.h"

#include <filesystem>

namespace Quirk {

    template<>
    class AssetImporter<Texture2D> : public AssetImporterBase<AssetImporter<Texture2D>> {
    public:
        static Ref<Texture2D> Import(const AssetMetadata& assetMeta);
        static Ref<Texture2D> CreateFromImage(const std::filesystem::path& filePath, const TextureProperties& properties = {});

    private:
        static TextureSwizzle GetDefaultSwizzleMask(ImageDataFormat format) noexcept;
    };

    using TextureImporter = AssetImporter<Texture2D>;

}
