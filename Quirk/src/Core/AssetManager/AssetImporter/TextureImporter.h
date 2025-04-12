

#pragma once

#include "Core/Core.h"
#include "Core/Assets/Texture.h"

namespace Quirk {

    class Texture2DImporter {
    public:
        static Ref<Texture2D> Import(const AssetMetadata& metadata);
    };


}
