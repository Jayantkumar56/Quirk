

#pragma once

#include "AssetImporter.h"
#include "EditorTexture.h"
#include "Core/RHI/Factory.h"


namespace Quirk {

    template<>
    class AssetImporter<EditorTexture2D> {
    public:
        static Ref<EditorTexture2D> Create(const EditorTexture2DSpec& spec, ConstView<RHI::Factory> factory)           noexcept;
        static Ref<EditorTexture2D> Import(const std::filesystem::path& texturePath, ConstView<RHI::Factory> factory)  noexcept;
        static bool                 Save(ConstView<EditorTexture2D> texture, const std::filesystem::path& texturePath) noexcept;

        static Ref<RHI::Texture2D> CreateTexture2D(
            const std::filesystem::path&   filePath,
            ConstView<RHI::Factory>        factory,
            const RHI::TextureProperties&  properties = {}
        ) noexcept;

    private:
        static RHI::TextureSwizzle GetDefaultSwizzleMask(RHI::ImgDataFmt format) noexcept;
    };

    using TextureImporter = AssetImporter<EditorTexture2D>;

}
