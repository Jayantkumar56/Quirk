

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

	Ref<Texture2D> Texture2D::Create(const std::string& filePath) {
		switch (Renderer::GetAPI()) {
			case RendererAPI::API::None:    QK_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:  return  std::make_shared<OpenGLTexture2D>(filePath);
		}

		QK_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	SubTexture2D::SubTexture2D(const std::string& filePath, int indexX, int indexY, int width, int height) {
		m_Texture = Texture2D::Create(filePath);

		m_CoordX = indexX;
		m_CoordY = indexY;
		m_TileWidth  = width;
		m_TileHeight = height;
	}

	SubTexture2D::SubTexture2D(const TextureSpecification& spec, int indexX, int indexY, int width, int height) {
		m_Texture = Texture2D::Create(spec);

		m_CoordX = indexX;
		m_CoordY = indexY;
		m_TileWidth = width;
		m_TileHeight = height;
	}

}
