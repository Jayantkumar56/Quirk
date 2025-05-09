
#pragma once

#include "Core/Assets/RenderAssets/Texture.h"
#include "glad/glad.h"

#include <filesystem>

namespace Quirk {

	GLint TextureWrapToOpenglWrap(TextureWrap wrap);
	GLint TexutureFilterToOpenglFilter(TextureFilter filter);

	class OpenGLTexture2D : public Texture2D {
	public:
		OpenGLTexture2D(Buffer dataBuffer, const TextureSpecification& specification);
		virtual ~OpenGLTexture2D();

		virtual bool operator ==(const Texture2D& other) const override { return m_RendererId == other.GetRendererId(); }

		virtual uint32_t GetRendererId() const noexcept override { return m_RendererId; }

		virtual void Bind(uint32_t slot = 0)            const override;
		virtual void SetData(void* data, uint32_t size) const override;

	private:
		uint32_t m_RendererId;
	};

}
