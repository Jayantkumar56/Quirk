


#include "Qkpch.h"
#include "Platform/OpenGL/OpenGLContext.h"
#include "Core/Frame/Window.h"
#include "Core/Frame/GraphicalContext.h"

namespace Quirk{

	GraphicalContext* GraphicalContext::Create(Window& window) {
		RendererAPI::API rendererAPI = RendererAPI::GetAPI();

		switch (rendererAPI) {
			case RendererAPI::API::None:    QK_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:  return new OpenGLContext(window);
		}

		QK_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	Scope<GraphicalContext> GraphicalContext::Create(Window& window, RendererAPI::API rendererAPI) {
		switch (rendererAPI) {
			case RendererAPI::API::None:    QK_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:  return CreateScope<OpenGLContext>(window);
		}

		QK_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	void GraphicalContext::Init(RendererAPI::API rendererAPI) {
		switch (rendererAPI) {
			case RendererAPI::API::None:    QK_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); break;
			case RendererAPI::API::OpenGL:  OpenGLContext::Init();
		}
	}

}
