

#pragma once

#include "EditorAsset.h"
#include "Core/Utility/TypeTraits.h"
#include "Core/RHI/Texture2D.h"


namespace Quirk {

    template<>
    struct EditorAssetSpec<RHI::Texture2D> {
        std::filesystem::path  ImagePath;
        RHI::TextureProperties TextureProps;
    };

    template<>
    struct EditorAssetMeta<RHI::Texture2D> {
        std::filesystem::path ImagePath;
    };

    using EditorTexture2DSpec = EditorAssetSpec<RHI::Texture2D>;
    using EditorTexture2DMeta = EditorAssetMeta<RHI::Texture2D>;
    using EditorTexture2D     = EditorAsset<RHI::Texture2D>;

}
