

#include "Qkpch.h"
#include "OpenGLTexture.h"
#include "stb_image.h"

namespace Quirk {

	static GLenum ImageFormatToGLDataFormat(ImageFormat format) {
		switch (format) {
			case ImageFormat::RGB8:  return GL_RGB;
			case ImageFormat::RGBA8: return GL_RGBA;
		}

		QK_CORE_ASSERT(false, "Incorrect Image Format!");
		return 0;
	}

	static GLenum ImageFormatToGLInternalFormat(ImageFormat format) {
		switch (format) {
			case ImageFormat::RGB8:  return GL_RGB8;
			case ImageFormat::RGBA8: return GL_RGBA8;
		}

		QK_CORE_ASSERT(false, "Incorrect Image Format!");
		return 0;
	}


    OpenGLTexture2D::OpenGLTexture2D(const TextureSpecification& spec) :
			m_Path(""),
			m_Width(spec.Width),
			m_Height(spec.Height)
	{
		m_InternalFormat = ImageFormatToGLInternalFormat(spec.Format);
		m_DataFormat = ImageFormatToGLDataFormat(spec.Format);

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererId);
		glTextureStorage2D(m_RendererId, 1, m_InternalFormat, m_Width, m_Height);

		glTextureParameteri(m_RendererId, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererId, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTextureParameteri(m_RendererId, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_RendererId, GL_TEXTURE_WRAP_T, GL_REPEAT);
    }

    OpenGLTexture2D::OpenGLTexture2D(const std::string& filePath) :
			m_Path(filePath)
	{
		int width, height, channels;

		stbi_set_flip_vertically_on_load(1);
		stbi_uc* data = stbi_load(filePath.c_str(), &width, &height, &channels, 0);
		QK_CORE_ASSERT(data, "Failed to load image!");
		m_Width = width;
		m_Height = height;

		GLenum internalFormat = 0, dataFormat = 0;
		if (channels == 4) {
			internalFormat = GL_RGBA8;
			dataFormat = GL_RGBA;
		}
		else if (channels == 3) {
			internalFormat = GL_RGB8;
			dataFormat = GL_RGB;
		}

		m_InternalFormat = internalFormat, m_DataFormat = dataFormat;
		QK_CORE_ASSERT(internalFormat & dataFormat, "Image format not supported!");

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererId);
		glTextureStorage2D(m_RendererId, 1, internalFormat, width, height);

		glTexParameteri(m_RendererId, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(m_RendererId, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		glTextureSubImage2D(m_RendererId, 0, 0, 0, width, height, dataFormat, GL_UNSIGNED_BYTE, data);
		stbi_image_free(data);
	}

	OpenGLTexture2D::~OpenGLTexture2D(){
		glDeleteTextures(1, &m_RendererId);
	}

	void OpenGLTexture2D::Bind(uint32_t slot) const {
		glBindTextureUnit(slot, m_RendererId);
	}

	void OpenGLTexture2D::SetData(void* data, uint32_t size) const {
		uint32_t bpp = m_DataFormat == GL_RGBA ? 4 : 3;
		QK_CORE_ASSERT(size == m_Width * m_Height * bpp, "Data must be entire texture!");
		glTextureSubImage2D(m_RendererId, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, data);
	}

}
