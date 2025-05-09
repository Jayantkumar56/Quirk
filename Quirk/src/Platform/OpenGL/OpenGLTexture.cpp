

#include "Qkpch.h"
#include "OpenGLTexture.h"
#include "stb_image.h"

namespace Quirk {

	GLint TextureWrapToOpenglWrap(TextureWrap wrap) {
		switch (wrap) {
			case TextureWrap::Repeat:		  return GL_REPEAT;
			case TextureWrap::MirroredRepeat: return GL_MIRRORED_REPEAT;
			case TextureWrap::ClampToEdge:	  return GL_CLAMP_TO_EDGE;
			case TextureWrap::ClampToBorder:  return GL_CLAMP_TO_BORDER;
		}

		QK_CORE_ASSERT(false, "Invalid TextureWrap type choosen!");
		return -1;
	}

	GLint TexutureFilterToOpenglFilter(TextureFilter filter) {
		switch (filter) {
			case TextureFilter::Linear:  return GL_LINEAR;
			case TextureFilter::Nearest: return GL_NEAREST;
		}

		QK_CORE_ASSERT(false, "Invalid TexutureFilter type choosen!");
		return -1;
	}

	static GLenum ImageDataFormatToGLDataFormat(ImageDataFormat format) {
		switch (format) {
			case ImageDataFormat::RGB:  return GL_RGB;
			case ImageDataFormat::RGBA: return GL_RGBA;
		}

		QK_CORE_ASSERT(false, "Incorrect Image Format!");
		return 0;
	}

	static GLenum ImageInternalFormatToGLInternalFormat(ImageInternalFormat format) {
		switch (format) {
			case ImageInternalFormat::RGB8:  return GL_RGB8;
			case ImageInternalFormat::RGBA8: return GL_RGBA8;
		}

		QK_CORE_ASSERT(false, "Incorrect Image Format!");
		return 0;
	}


    OpenGLTexture2D::OpenGLTexture2D(Buffer dataBuffer, const TextureSpecification& spec) :
            Texture2D(spec)
	{
		GLenum dataFormat     = ImageDataFormatToGLDataFormat(spec.DataFormat);
		GLenum internalFormat = ImageInternalFormatToGLInternalFormat(spec.GpuInternalFormat);

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
            (void*)dataBuffer.Data
        );
    }

	OpenGLTexture2D::~OpenGLTexture2D(){
		glDeleteTextures(1, &m_RendererId);
	}

	void OpenGLTexture2D::Bind(uint32_t slot) const {
		glBindTextureUnit(slot, m_RendererId);
	}

	void OpenGLTexture2D::SetData(void* data, uint32_t size) const {
        uint32_t bpp = m_Specification.Channels;
		QK_CORE_ASSERT(size == m_Specification.Width * m_Specification.Height * bpp, "Data must be entire texture!");

		glTextureSubImage2D(
            m_RendererId,
            0, 0, 0,
            m_Specification.Width,
            m_Specification.Height,
            ImageDataFormatToGLDataFormat(m_Specification.DataFormat),
            GL_UNSIGNED_BYTE,
            data
        );
	}

}
