

#pragma once

#include "Core/Core.h"
#include "Core/Assets/Asset.h"
#include "Core/Utility/Buffer.h"

#include <utility>
#include <string>

namespace Quirk {

    enum class ImageDataFormat : uint8_t {          // Data upload formats
        None = 0,

        Red,
        RG,
        RGB,
        RGBA,

        // Depth and Stencil formats
        Depth,
        Stencil,
    };

    enum class ImageInternalFormat : uint8_t {      // GPU storage format
        None = 0,

        R8,
        RG8,
        RGB8,
        RGBA8,

        R16F,
        RG16F,
        RGB16F,
        RGBA16F,

        R32F,
        RG32F,
        RGB32F,
        RGBA32F,

        SRGB8,
        SRGB8_Alpha8,

        Depth24Stencil8,
        Depth32F
    };

    enum class Swizzle : uint8_t {
        None,
        Red,
        Green,
        Blue,
        Alpha,
        Zero,
        One
    };

    struct TextureSwizzle {
        Swizzle R{ Swizzle::None };
        Swizzle G{ Swizzle::None };
        Swizzle B{ Swizzle::None };
        Swizzle A{ Swizzle::None };

        bool IsValid() const noexcept {
            return R != Swizzle::None && G != Swizzle::None && B != Swizzle::None && A != Swizzle::None;
        }
    };

	enum class TextureFilter : uint8_t {
		Linear,
        Nearest
	};

	enum class TextureWrap : uint8_t {
		Repeat,
        MirroredRepeat,
		ClampToEdge,
        ClampToBorder
	};

    struct TextureProperties {
        TextureFilter MinFilter{ TextureFilter::Linear };
        TextureFilter MagFilter{ TextureFilter::Linear };

        TextureWrap WrapS{ TextureWrap::Repeat };
        TextureWrap WrapT{ TextureWrap::Repeat };

        TextureSwizzle SwizzleMask{ };
    };

    struct TextureSpecification {
        int Width    { 1 };
        int Height   { 1 };
        int Channels { 4 };

        ImageDataFormat     DataFormat        { ImageDataFormat::RGBA      };
        ImageInternalFormat GpuInternalFormat { ImageInternalFormat::RGBA8 };

        TextureFilter MinFilter{ TextureFilter::Linear };
        TextureFilter MagFilter{ TextureFilter::Linear };

        TextureWrap WrapS{ TextureWrap::Repeat };
        TextureWrap WrapT{ TextureWrap::Repeat };

        TextureSwizzle SwizzleMask{ };
    };

    class Texture2D : public Asset {
    public:
        Texture2D(const TextureSpecification& spec) : m_Specification(spec) {}
        virtual ~Texture2D() = default;

        virtual bool operator ==(const Texture2D& other) const = 0;

        virtual uint32_t GetRendererId()                const noexcept = 0;
        virtual void Bind(uint32_t slot = 0)            const = 0;

        // TODO: THINK ABOUT THIS
        virtual void SetData(void* data, uint32_t size) const = 0;

        inline uint32_t             GetWidth()  const noexcept { return m_Specification.Width; }
        inline uint32_t             GetHeight() const noexcept { return m_Specification.Height; }
        inline TextureSpecification GetSpec()   const noexcept { return m_Specification; }

        // maybe think about this situation
        static Ref<Texture2D> Create(Buffer dataBuffer, const TextureSpecification& spec);

    protected:
        TextureSpecification m_Specification;
    };

}
