

#pragma once

#include "AssetImporter.h"
#include "Core/Assets/TextureAsset.h"


namespace Quirk {

    namespace RHI {
        class Factory;
    }

    template<>
    class AssetImporter<TextureAsset> {
    public:
        static Ref<TextureAsset> CreateFromSource(const std::filesystem::path& filePath, ConstView<RHI::Factory> factory);
        static Ref<TextureAsset> Import(const std::filesystem::path& textureAssetPath, ConstView<RHI::Factory> factory);
        static bool              Save(ConstView<TextureAsset> texture, const std::filesystem::path& texturePath);

        static Ref<RHI::Texture2D> CreateTexture2D(
            const std::filesystem::path& filePath,
            ConstView<RHI::Factory>        factory,
            const RHI::TextureProperties& properties = {}
        ) noexcept;

        static std::string GetAssetFileExtension() { return ".qk_texture"; }

    private:
        static RHI::TextureSwizzle GetDefaultSwizzleMask(RHI::ImgDataFmt format) noexcept;
    };

    using TextureImporter = AssetImporter<TextureAsset>;

}
