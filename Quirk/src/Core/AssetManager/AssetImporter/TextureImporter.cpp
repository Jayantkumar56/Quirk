

#include "TextureImporter.h"
#include "Core/Utility/Buffer.h"
#include "Core/Reflection/Registrations/TextureEnums.h"
#include "Core/Reflection/Registrations/EditorTexture2D.h"
#include "Core/Serialization/Serialization.h"

#include "stb_image.h"


namespace Quirk {

    Ref<EditorTexture2D> AssetImporter<EditorTexture2D>::Create(const EditorTexture2DSpec& spec, ConstView<RHI::Factory> factory) noexcept {
        return CreateRef<EditorTexture2D>(
            CreateTexture2D(spec.ImagePath, factory, spec.TextureProps),
            EditorTexture2DMeta(spec.ImagePath)
        );
    }

    Ref<EditorTexture2D> AssetImporter<EditorTexture2D>::Import(const std::filesystem::path& texturePath, ConstView<RHI::Factory> factory) noexcept {
        try {
            EditorTexture2DSpec textureSpec = Serialization::Deserialize<EditorTexture2DSpec>(texturePath);
            return Create(textureSpec, factory);
        }
        catch (const std::exception& e) {
            QK_ERROR("Importing EditorTexture2D with path {0} failed with error: {1}", texturePath.string(), e.what());
            return nullptr;
        }
        catch (...) {
            QK_ERROR("Importing EditorTexture2D with path {0} failed with some unknown error.", texturePath.string());
            return nullptr;
        }
    }

    bool AssetImporter<EditorTexture2D>::Save(ConstView<EditorTexture2D> texture, const std::filesystem::path& texturePath) noexcept {
        EditorTexture2DSpec textureSpec{
            texture->MetaData.ImagePath,
            texture->Asset->GetProperties()
        };

        try {
            if (!Serialization::Serialize(textureSpec, texturePath)) {
                QK_ERROR("Saving EditorTexture2D with path {} failed.", texturePath.string());
                return false;
            }
        }
        catch (...) {
            QK_ERROR("Saving EditorTexture2D with path {} failed with some unknown error.", texturePath.string());
            return false;
        }

        return true;
    }

    Ref<RHI::Texture2D> AssetImporter<EditorTexture2D>::CreateTexture2D(
            const std::filesystem::path&  filePath,
            ConstView<RHI::Factory>       factory,
            const RHI::TextureProperties& properties
    ) noexcept 
    {
        stbi_uc*    data = nullptr;
        std::string file = filePath.string();

        RHI::TextureSpec textureSpec{
            .Width             { 0                          },
            .Height            { 0                          },
            .Channels          { 0                          },
            .DataFormat        { RHI::ImgDataFmt::RGBA      },
            .GpuInternalFormat { RHI::ImgInternalFmt::RGBA8 },
            .MinFilter         { properties.MinFilter       },
            .MagFilter         { properties.MagFilter       },
            .WrapS             { properties.WrapS           },
            .WrapT             { properties.WrapT           },
            .SwizzleMask       { properties.SwizzleMask     }
        };

        // loading image data from the file
        {
            int width = 0, height = 0, channels = 0;

            stbi_set_flip_vertically_on_load(1);
            data = stbi_load(file.c_str(), &width, &height, &channels, 0);

            textureSpec.Width    = static_cast<uint32_t>(width);
            textureSpec.Height   = static_cast<uint32_t>(height);
            textureSpec.Channels = static_cast<uint32_t>(channels);

            QK_CORE_WARN_IF(data == nullptr, "Failed to load image with path {0}", file);
        }

        // loading image with 8 bits (1 byte) per channel, so don't need to multiply by 1
        size_t dataSize = static_cast<size_t>(textureSpec.Width * textureSpec.Height * textureSpec.Channels);

        // now the image data is owned by this Buffer object
        Buffer dataBuffer((void*)data, dataSize);

        switch (textureSpec.Channels) {
            case 1: {
                textureSpec.DataFormat = RHI::ImgDataFmt::Red;
                textureSpec.GpuInternalFormat = RHI::ImgInternalFmt::R8;
                break;
            }

            case 2: {
                textureSpec.DataFormat = RHI::ImgDataFmt::RG;
                textureSpec.GpuInternalFormat = RHI::ImgInternalFmt::RG8;
                break;
            }

            case 3: {
                textureSpec.DataFormat = RHI::ImgDataFmt::RGB;
                textureSpec.GpuInternalFormat = RHI::ImgInternalFmt::RGB8;
                break;
            }

            case 4: {
                textureSpec.DataFormat = RHI::ImgDataFmt::RGBA;
                textureSpec.GpuInternalFormat = RHI::ImgInternalFmt::RGBA8;
                break;
            }

            default: {
                QK_CORE_WARN("Unsupported number of channels!");
                return nullptr;
            }
        }

        if (!textureSpec.SwizzleMask.IsValid()) {
            textureSpec.SwizzleMask = GetDefaultSwizzleMask(textureSpec.DataFormat);
        }

        return factory->CreateTexture(dataBuffer, textureSpec);
    }

    RHI::TextureSwizzle AssetImporter<EditorTexture2D>::GetDefaultSwizzleMask(RHI::ImgDataFmt format) noexcept {
        switch (format) {
            case RHI::ImgDataFmt::Red:  return { RHI::Swizzle::Red, RHI::Swizzle::Red,   RHI::Swizzle::Red,  RHI::Swizzle::One   };
            case RHI::ImgDataFmt::RG:   return { RHI::Swizzle::Red, RHI::Swizzle::Green, RHI::Swizzle::Zero, RHI::Swizzle::One   };
            case RHI::ImgDataFmt::RGB:  return { RHI::Swizzle::Red, RHI::Swizzle::Green, RHI::Swizzle::Blue, RHI::Swizzle::One   };
            case RHI::ImgDataFmt::RGBA: return { RHI::Swizzle::Red, RHI::Swizzle::Green, RHI::Swizzle::Blue, RHI::Swizzle::Alpha };
        }

        QK_WARN("No TextureSwizzle support for given ImageDataFormat {0}", static_cast<int>(format));
        return { RHI::Swizzle::Red, RHI::Swizzle::Green, RHI::Swizzle::Blue, RHI::Swizzle::Alpha };
    }

}
