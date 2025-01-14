


#include "Qkpch.h"
#include "Platform/OpenGL/OpenGLContext.h"
#include "Core/Application/Window.h"
#include "Core/Renderer/GraphicalContext.h"

namespace Quirk{

	Ref<GraphicalContext> GraphicalContext::Create(Window& window) {
		RendererAPI::API rendererAPI = RendererAPI::GetAPI();

		switch (rendererAPI) {
			case RendererAPI::API::None:    QK_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:  return CreateRef<OpenGLContext>(window);
		}

		QK_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	Ref<GraphicalContext> GraphicalContext::Create(Window& window, RendererAPI::API rendererAPI) {
		switch (rendererAPI) {
			case RendererAPI::API::None:    QK_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:  return CreateRef<OpenGLContext>(window);
		}

		QK_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	void GraphicalContext::Init(RendererAPI::API rendererAPI) {
		switch (rendererAPI) {
			case RendererAPI::API::None:    QK_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
			case RendererAPI::API::OpenGL:  OpenGLContext::Init();
		}
	}

}
