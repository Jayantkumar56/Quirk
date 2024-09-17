
#pragma once

#include "Core/Renderer/Texture.h"
#include "glad/glad.h"

namespace Quirk {

	class OpenGLTexture2D : public Texture2D{
	public:
		OpenGLTexture2D(const TextureSpecification& specification);
		OpenGLTexture2D(const std::string& filePath);
		virtual ~OpenGLTexture2D();

		virtual bool operator ==(const Texture& other) const override { return m_RendererId == other.GetRendererId(); }

		virtual std::string GetPath()	 const override { return m_Path; }
		virtual uint32_t GetRendererId() const override { return m_RendererId; }
		virtual uint32_t GetWidth()		 const override { return m_Width; }
		virtual uint32_t GetHeight()	 const override { return m_Height; }

		virtual void Bind(uint32_t slot = 0) const override;
		virtual void SetData(void* data, uint32_t size) const override;

	private:
		std::string m_Path;
		uint32_t m_Width, m_Height;
		uint32_t m_RendererId;
		GLenum m_InternalFormat, m_DataFormat;
	};

}
