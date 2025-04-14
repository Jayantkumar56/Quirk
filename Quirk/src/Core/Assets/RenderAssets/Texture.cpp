

#include "Qkpch.h"

#include "Texture.h"
#include "Core/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLTexture.h"

#include "Core/AssetManager/AssetImporter/TextureImporter.h"

namespace Quirk {

    Ref<Texture2D> Texture2D::Create(const std::filesystem::path& filePath) {
        return Texture2DImporter::Import(filePath);
    }

    Ref<Texture2D> Texture2D::Create(Buffer dataBuffer, const TextureSpecification& spec) {
		switch (Renderer::GetAPI()) {
			case RendererAPI::API::None:    QK_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:  return std::make_shared<OpenGLTexture2D>(dataBuffer, spec);
		}

		QK_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
    }

}
