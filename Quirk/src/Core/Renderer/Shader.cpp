

#include "Qkpch.h"

#include "Shader.h"
#include "Platform/OpenGL/OpenGLShader.h"

#include "Renderer.h"
#include "Core/Core.h"

namespace Quirk {

	Shader* Shader::Create(const std::string& vertexSrc, const std::string& fragmentSrc) {
		RendererAPI::API currentAPI = Renderer::GetAPI();

		switch (currentAPI) {
			case Quirk::RendererAPI::API::None:		QK_CORE_ASSERT(false, "RendererAPI::API::None is not supported right now!"); return nullptr;
			case Quirk::RendererAPI::API::OpenGL:	return new OpenGLShder(vertexSrc, fragmentSrc);
		}

		QK_CORE_ASSERT(false, "Invalid Renderer API Selected!");
		return nullptr;
	}

}
