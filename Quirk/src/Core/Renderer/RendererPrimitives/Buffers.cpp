

#include "qkpch.h"
#include "Buffers.h"

#include "Core/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLBuffers.h"

namespace Quirk {

	Ref<VertexBuffer> VertexBuffer::Create() {
		switch (Renderer::GetAPI()) {
		case RendererAPI::API::None:    QK_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:  return std::make_shared<OpenGLVertexBuffer>();
		}

		QK_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	Ref<VertexBuffer> VertexBuffer::Create(uint32_t size) {
		switch (Renderer::GetAPI()) {
		case RendererAPI::API::None:    QK_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:  return std::make_shared<OpenGLVertexBuffer>(size);
		}

		QK_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	Ref<VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t size) {
		switch (Renderer::GetAPI()) {
			case RendererAPI::API::None:    QK_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:  return std::make_shared<OpenGLVertexBuffer>(vertices, size);
		}

		QK_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	Ref<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t count) {
		switch (Renderer::GetAPI()) {
			case RendererAPI::API::None:    QK_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:  return std::make_shared<OpenGLIndexBuffer>(indices, count);
		}

		QK_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}

