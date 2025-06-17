

#include "Qkpch.h"

#include "OpenGLTexture2D.h"
#include "Core/Core.h"

#include "glad/glad.h"


namespace Quirk::OpenGL {

    // === Begin: Forward Declaration =======

    static GLenum ImgDataFmtToGLDataFmt         ( RHI::ImgDataFmt     format );
    static GLenum ImgInternalFmtToGLInternalFmt ( RHI::ImgInternalFmt format );

    // === End:   Forward Declaration =======


    Texture2D::Texture2D(BufferView data, const RHI::TextureSpec& spec) noexcept :
            RHI::Texture2D(spec)
    {
        GLenum dataFormat     = ImgDataFmtToGLDataFmt         ( spec.DataFormat        );
		GLenum internalFormat = ImgInternalFmtToGLInternalFmt ( spec.GpuInternalFormat );

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererId);
		glTextureStorage2D(m_RendererId, 1, internalFormat, spec.Width, spec.Height);

		glTextureParameteri(m_RendererId, GL_TEXTURE_MIN_FILTER, TexutureFilterToOpenglFilter(spec.MinFilter));
		glTextureParameteri(m_RendererId, GL_TEXTURE_MAG_FILTER, TexutureFilterToOpenglFilter(spec.MagFilter));

		glTextureParameteri(m_RendererId, GL_TEXTURE_WRAP_S, TextureWrapToOpenglWrap(spec.WrapS));
		glTextureParameteri(m_RendererId, GL_TEXTURE_WRAP_T, TextureWrapToOpenglWrap(spec.WrapT));

		glTextureSubImage2D(
            m_RendererId, 
            0, 0, 0, 
            spec.Width,
            spec.Height,
            dataFormat, 
            GL_UNSIGNED_BYTE, 
            data.Get()
        );
    }

    Texture2D::~Texture2D() {
        glDeleteTextures(1, &m_RendererId);
    }

    void Texture2D::Bind(uint32_t slot) const noexcept {
        glBindTextureUnit(slot, m_RendererId);
    }

    void Texture2D::SetData(BufferView data, uint32_t xOffset, uint32_t yOffset, uint32_t width, uint32_t height) noexcept {
        QK_CORE_ASSERT(width > 0 && height > 0, "Cannot upload zero-sized texture region");

        if (xOffset + width > m_Specification.Width || yOffset + height > m_Specification.Height) {
            QK_CORE_ERROR(
                "Texture subimage out of bounds: attempted upload region [{}, {}] to [{}, {}] exceeds texture size [{} x {}]",
                xOffset, yOffset, xOffset + width, yOffset + height,
                m_Specification.Width, m_Specification.Height
            );

            return;
        }

        uint32_t bpp          = m_Specification.Channels;
        uint32_t expectedSize = width * height * bpp;

        QK_CORE_ASSERT(
            data.Size() == expectedSize,
            "Data size mismatch: expected {} bytes ({}x{}x{}), got {} bytes",
            expectedSize, width, height, bpp, data.Size()
        );

        glTextureSubImage2D(
            m_RendererId,
            0,
            xOffset,
            yOffset,
            width,
            height,
            ImgDataFmtToGLDataFmt(m_Specification.DataFormat),
            GL_UNSIGNED_BYTE,
            data.Get()
        );
    }

    bool Texture2D::Equal(const RHI::Texture2D& other) const noexcept {
        if (auto* otherTexture = dynamic_cast<const Texture2D*>(&other)) {
            return m_RendererId == otherTexture->m_RendererId;
        }

        return false;
    }


    //=============================================================================================================================
    //--------- RHI::Texture2D enum to OpenGL enum conversions --------------------------------------------------------------------

    int TextureWrapToOpenglWrap(RHI::TextureWrap wrap) {
        switch (wrap) {
            case RHI::TextureWrap::Repeat:         return GL_REPEAT;
            case RHI::TextureWrap::MirroredRepeat: return GL_MIRRORED_REPEAT;
            case RHI::TextureWrap::ClampToEdge:    return GL_CLAMP_TO_EDGE;
            case RHI::TextureWrap::ClampToBorder:  return GL_CLAMP_TO_BORDER;
        }

        QK_CORE_ASSERT(false, "Invalid TextureWrap type choosen!");
        return -1;
    }

    int TexutureFilterToOpenglFilter(RHI::TextureFilter filter) {
        switch (filter) {
            case RHI::TextureFilter::Linear:  return GL_LINEAR;
            case RHI::TextureFilter::Nearest: return GL_NEAREST;
        }

        QK_CORE_ASSERT(false, "Invalid TexutureFilter type choosen!");
        return -1;
    }

    static GLenum ImgDataFmtToGLDataFmt(RHI::ImgDataFmt format) {
		switch (format) {
            case RHI::ImgDataFmt::RGB:  return GL_RGB;
			case RHI::ImgDataFmt::RGBA: return GL_RGBA;
		}

		QK_CORE_ASSERT(false, "Incorrect Image Format!");
		return 0;
	}

	static GLenum ImgInternalFmtToGLInternalFmt(RHI::ImgInternalFmt format) {
		switch (format) {
			case RHI::ImgInternalFmt::RGB8:  return GL_RGB8;
			case RHI::ImgInternalFmt::RGBA8: return GL_RGBA8;
		}

		QK_CORE_ASSERT(false, "Incorrect Image Format!");
		return 0;
	}

    //_____________________________________________________________________________________________________________________________

}
