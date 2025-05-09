

#pragma once

#include "Core/Assets/RenderAssets/Texture.h"
#include "AssetImporter.h"

#include <filesystem>


namespace Quirk {

    template <>
    struct EditorAsset<Texture2D> {
        std::filesystem::path TexturePath;
        std::filesystem::path ImagePath;
        Ref<Texture2D>        Texture;

        inline Ref<Texture2D> GetAsset() const noexcept { return Texture;            }
        inline bool           IsLoaded() const noexcept { return Texture != nullptr; }
    };

    using EditorTexture2D = EditorAsset<Texture2D>;

}
