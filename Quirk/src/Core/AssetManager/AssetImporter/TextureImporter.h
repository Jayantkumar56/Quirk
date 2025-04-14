

#pragma once

#include "Core/Core.h"
#include "Core/Assets/RenderAssets/Texture.h"

#include <filesystem>

namespace Quirk {

    class Texture2DImporter {
    public:
        static Ref<Texture2D> ImportFromMetadata(const std::filesystem::path& filePath);
        static Ref<Texture2D> Import(const std::filesystem::path& filePath, const TextureProperties& properties = {});

    private:
        static TextureSwizzle GetDefaultSwizzleMask(ImageDataFormat format) noexcept;
    };

}
