

#pragma once

#include "Core/Core.h"
#include "Core/Assets/RenderAssets/Texture.h"
#include "AssetImporter.h"
#include "EditorTexture.h"
#include "Core/Serialization/EditorTextureSerialization.h"

#include <filesystem>

namespace Quirk {

    template<>
    class AssetImporter<Texture2D> {
    public:
        // will modify the provided EditorTexture2D to set the texture object
        static inline AssetHandle Create(EditorTexture2D& outAsset) {
            if (outAsset.ImagePath == "" || outAsset.TexturePath == "") {
                QK_CORE_ERROR("Invalid ImagePath or TexturePath provided");
                return AssetHandle{};
            }
            
            if (!(outAsset.Texture = CreateFromImage(outAsset.TexturePath))) {
                QK_CORE_ERROR("Failed to create texture from Image {0}", outAsset.ImagePath.string());
                return AssetHandle{};
            }

            if (!Serializer<EditorTexture2D>::Serialize(outAsset))
                QK_CORE_ERROR("Failed to save created Texture at {0}", outAsset.TexturePath.string());

            return outAsset.Texture->GetHandle();
        }

        // will modify the provided EditorTexture2D to set the texture object and the meta
        static inline void Import(EditorTexture2D& outAsset) {
            if (!Deserializer<EditorTexture2D>::Deserialize(outAsset))
                QK_CORE_ERROR("Failed to load Texture {0}", outAsset.TexturePath.string());
        }

        // saves the Texture2D in a texture file (a yaml based file)
        static inline void Save(const EditorTexture2D& texture) {
            if (!Serializer<EditorTexture2D>::Serialize(texture))
                QK_CORE_ERROR("Failed to save Texture at {0}", texture.TexturePath.string());
        }

        // creates texture from raw image file path and provided TextureProperties
        static Ref<Texture2D> CreateFromImage(const std::filesystem::path& filePath, const TextureProperties& properties = {});

        // creates texture from raw image file path and provided TextureProperties and set the given handle
        static inline Ref<Texture2D> CreateFromImage(
                const AssetHandle             handle, 
                const std::filesystem::path&  filePath, 
                const TextureProperties&      properties = {}
        ){
            Ref<Texture2D> asset = CreateFromImage(filePath, properties);
            asset->SetHandle(handle);
            return asset;
        }

    private:
        static TextureSwizzle GetDefaultSwizzleMask(ImageDataFormat format) noexcept;
    };

    using TextureImporter = AssetImporter<Texture2D>;

}
