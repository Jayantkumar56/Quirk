

#include "Qkpch.h"

#include "TextureImporter.h"
#include "Core/Utility/Buffer.h"

#include "stb_image.h"

namespace Quirk {

    Ref<Texture2D> AssetImporter<Texture2D>::CreateFromImage(const std::filesystem::path& filePath, const TextureProperties& properties) {
        stbi_uc*    data = nullptr;
        std::string file = filePath.string();

        TextureSpecification textureSpec{
            .Width             { 0                          },
            .Height            { 0                          },
            .DataFormat        { ImageDataFormat::RGBA      },
            .GpuInternalFormat { ImageInternalFormat::RGBA8 },
            .MinFilter         { properties.MinFilter       },
            .MagFilter         { properties.MagFilter       },
            .WrapS             { properties.WrapS           },
            .WrapT             { properties.WrapT           },
            .SwizzleMask       { properties.SwizzleMask     }
        };

        // loading image data from the file
        {
            stbi_set_flip_vertically_on_load(1);
            data = stbi_load(file.c_str(), &textureSpec.Width, &textureSpec.Height, &textureSpec.Channels, 0);

            QK_CORE_WARN_IF(data == nullptr, "Failed to load image with path {0}", file);
        }

        // loading image with 8 bits (1 byte) per channel, so don't need to multiply by 1
        size_t dataSize = static_cast<size_t>(textureSpec.Width * textureSpec.Height * textureSpec.Channels);

        // now the image data is owned by this Buffer object
        Buffer dataBuffer((void*)data, dataSize);

        switch (textureSpec.Channels) {
            case 1: {
                textureSpec.DataFormat = ImageDataFormat::Red;
                textureSpec.GpuInternalFormat = ImageInternalFormat::R8;
                break;
            }

            case 2: {
                textureSpec.DataFormat = ImageDataFormat::RG;
                textureSpec.GpuInternalFormat = ImageInternalFormat::RG8;
                break;
            }

            case 3: {
                textureSpec.DataFormat = ImageDataFormat::RGB;
                textureSpec.GpuInternalFormat = ImageInternalFormat::RGB8;
                break;
            }

            case 4: {
                textureSpec.DataFormat = ImageDataFormat::RGBA;
                textureSpec.GpuInternalFormat = ImageInternalFormat::RGBA8;
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

        return Texture2D::Create(std::move(dataBuffer), textureSpec);
    }

    TextureSwizzle AssetImporter<Texture2D>::GetDefaultSwizzleMask(ImageDataFormat format) noexcept {
        switch (format) {
            case ImageDataFormat::Red:  return { Swizzle::Red, Swizzle::Red,   Swizzle::Red,  Swizzle::One   };
            case ImageDataFormat::RG:   return { Swizzle::Red, Swizzle::Green, Swizzle::Zero, Swizzle::One   };
            case ImageDataFormat::RGB:  return { Swizzle::Red, Swizzle::Green, Swizzle::Blue, Swizzle::One   };
            case ImageDataFormat::RGBA: return { Swizzle::Red, Swizzle::Green, Swizzle::Blue, Swizzle::Alpha };
        }

        QK_WARN("No TextureSwizzle support for given ImageDataFormat {0}", static_cast<int>(format));
        return { Swizzle::Red, Swizzle::Green, Swizzle::Blue, Swizzle::Alpha };
    }

}

