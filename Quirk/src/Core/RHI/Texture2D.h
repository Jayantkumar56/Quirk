

#pragma once

#include "Core/Utility/Buffer.h"

#include <utility>


namespace Quirk::RHI {

    enum class ImgDataFmt : uint8_t {          // Data upload formats
        None = 0,

        Red,
        RG,
        RGB,
        RGBA,

        // Depth and Stencil formats
        Depth,
        Stencil,
    };

    enum class ImgInternalFmt : uint8_t {      // GPU storage format
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

    struct TextureSpec {
        uint32_t Width    { 1 };
        uint32_t Height   { 1 };
        uint32_t Channels { 4 };

        ImgDataFmt     DataFormat        { ImgDataFmt::RGBA      };
        ImgInternalFmt GpuInternalFormat { ImgInternalFmt::RGBA8 };

        TextureFilter  MinFilter         { TextureFilter::Linear };
        TextureFilter  MagFilter         { TextureFilter::Linear };
                                         
        TextureWrap    WrapS             { TextureWrap::Repeat   };
        TextureWrap    WrapT             { TextureWrap::Repeat   };

        TextureSwizzle SwizzleMask{ };
    };

    class Texture2D {
    public:
        Texture2D(const TextureSpec& spec) noexcept : m_Specification(spec) {}
        virtual ~Texture2D() = default;

        inline bool operator ==(const Texture2D& other) const noexcept { return Equal(other); }

        virtual uint32_t GetRendererId()     const noexcept = 0;
        virtual void Bind(uint32_t slot = 0) const noexcept = 0;

        virtual void SetData(
            BufferView data, 
            uint32_t   xOffset, 
            uint32_t   yOffset, 
            uint32_t   width, 
            uint32_t   height 
        ) noexcept = 0;

        inline void SetData(BufferView data) noexcept {
            SetData(data, 0, 0, m_Specification.Width, m_Specification.Height);
        }

        inline       uint32_t     GetWidth()  const noexcept { return m_Specification.Width;  }
        inline       uint32_t     GetHeight() const noexcept { return m_Specification.Height; }
        inline const TextureSpec& GetSpec()   const noexcept { return m_Specification;        }

    private:
        virtual inline bool Equal(const Texture2D& other) const noexcept = 0;

    protected:
        TextureSpec m_Specification;
    };

}
