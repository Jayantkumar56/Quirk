

#include "Qkpch.h"

#include "RendererAPI.h"
#include "Core/Core.h"

#include "Core/Application/Application.h"
#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Quirk {

	RendererAPI::API RendererAPI::s_API = RendererAPI::API::None; 
	GraphicalContext* RendererAPI::s_GraphicalContext = nullptr;

	RendererAPI::RendererAPI(GraphicalContext* context, API rendererAPI) {
		s_GraphicalContext = context; 
		s_API = rendererAPI;
	}

}
