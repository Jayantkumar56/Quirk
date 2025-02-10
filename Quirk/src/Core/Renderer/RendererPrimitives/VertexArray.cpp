
#include "Qkpch.h"

#include "VertexArray.h"
#include "Core/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace Quirk {

	Ref<VertexArray> VertexArray::Create() {
		switch (Renderer::GetAPI()) {
			case RendererAPI::API::None:    QK_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:  return std::make_shared<OpenGLVertexArray>();
		}

		QK_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}
