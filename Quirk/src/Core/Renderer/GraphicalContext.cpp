


#include "Qkpch.h"
#include "Core/Renderer/GraphicalContext.h"
#include "Platform/OpenGL/OpenGLContext.h"

namespace Quirk{

	GraphicalContext* GraphicalContext::Create(RendererAPI::API rendererAPI) {
		switch (rendererAPI) {
			case RendererAPI::API::None:    QK_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:  return new OpenGLContext();
		}

		QK_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}
