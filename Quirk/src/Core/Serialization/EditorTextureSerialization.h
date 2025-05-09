

#pragma once

#include "Serialization.h"
#include "Core/AssetManager/AssetImporter/EditorTexture.h"


namespace Quirk {

    template<>
    struct Serializer<EditorTexture2D> {
        static bool Serialize(const EditorTexture2D& outTexture);
    };

    template<>
    struct Deserializer<EditorTexture2D> {
        static bool Deserialize(EditorTexture2D& texture);
    };

}
