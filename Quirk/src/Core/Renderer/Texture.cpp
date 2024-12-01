

#include "Qkpch.h"

#include "Texture.h"
#include "Core/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLTexture.h"

namespace Quirk {

	Ref<Texture2D> Texture2D::Create(const TextureSpecification& spec) {
		switch (Renderer::GetAPI()) {
			case RendererAPI::API::None:    QK_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:  return std::make_shared<OpenGLTexture2D>(spec);
		}

		QK_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	Ref<Texture2D> Texture2D::Create(const std::filesystem::path& filePath) {
		switch (Renderer::GetAPI()) {
			case RendererAPI::API::None:    QK_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:  return  std::make_shared<OpenGLTexture2D>(filePath);
		}

		QK_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	SubTexture2D::SubTexture2D(Ref<Texture2D>& texture, uint16_t coordX, uint16_t coordY, uint16_t width, uint16_t height) {
		m_Texture	 = texture;
		m_CoordX	 = coordX;
		m_CoordY	 = coordY;
		m_TileWidth  = width;
		m_TileHeight = height;
	}

	SubTexture2D::SubTexture2D(const std::string& filePath, uint16_t coordX, uint16_t coordY, uint16_t width, uint16_t height) {
		m_Texture	 = Texture2D::Create(filePath);
		m_CoordX	 = coordX;
		m_CoordY	 = coordY;
		m_TileWidth  = width;
		m_TileHeight = height;
	}

	SubTexture2D::SubTexture2D(const TextureSpecification& spec, uint16_t coordX, uint16_t coordY, uint16_t width, uint16_t height) {
		m_Texture	 = Texture2D::Create(spec);
		m_CoordX	 = coordX;
		m_CoordY	 = coordY;
		m_TileWidth  = width;
		m_TileHeight = height;
	}

}
