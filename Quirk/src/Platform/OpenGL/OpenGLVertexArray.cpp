

#include "Qkpch.h"
#include "OpenGLVertexArray.h"
#include "glad/glad.h"

namespace Quirk {

	static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type) {
		switch (type) {
			case Quirk::ShaderDataType::Float:    return GL_FLOAT;
			case Quirk::ShaderDataType::Float2:   return GL_FLOAT;
			case Quirk::ShaderDataType::Float3:   return GL_FLOAT;
			case Quirk::ShaderDataType::Float4:   return GL_FLOAT;
			case Quirk::ShaderDataType::Mat3:     return GL_FLOAT;
			case Quirk::ShaderDataType::Mat4:     return GL_FLOAT;
			case Quirk::ShaderDataType::Int:      return GL_INT;
			case Quirk::ShaderDataType::Int2:     return GL_INT;
			case Quirk::ShaderDataType::Int3:     return GL_INT;
			case Quirk::ShaderDataType::Int4:     return GL_INT;
			case Quirk::ShaderDataType::Bool:     return GL_BOOL;
		}

		QK_CORE_ASSERT(false, "Unknown ShaderDataType!");
		return 0;
	}

	OpenGLVertexArray::OpenGLVertexArray() {
		m_IndexBuffer = nullptr;
		glCreateVertexArrays(1, &m_RendererId);
	}

	OpenGLVertexArray::~OpenGLVertexArray() {
		glDeleteVertexArrays(1, &m_RendererId);

		for (VertexBuffer* vBuffer : m_VertexBuffers) {
			delete vBuffer;
		}
	}

	void Quirk::OpenGLVertexArray::Bind() const {
		glBindVertexArray(m_RendererId);
	}

	void Quirk::OpenGLVertexArray::Unbind() const {
		glBindVertexArray(0);
	}

	void Quirk::OpenGLVertexArray::AddVertexBuffer(VertexBuffer* vertexBuffer) {
		QK_CORE_ASSERT(vertexBuffer->GetLayout().GetElements().size(), "Vertex Buffer has no layout!");

		glBindVertexArray(m_RendererId);
		vertexBuffer->Bind();

		uint32_t index = 0;
		const auto& layout = vertexBuffer->GetLayout();

		for (const auto& element : layout) {
			glEnableVertexAttribArray(index);

			glVertexAttribPointer(
				index,
				element.GetComponentCount(),
				ShaderDataTypeToOpenGLBaseType(element.Type),
				element.Normalized ? GL_TRUE : GL_FALSE,
				layout.GetStride(),
				(const void*)element.Offset
			);

			index++;
		}

		m_VertexBuffers.push_back(vertexBuffer);
	}

	void Quirk::OpenGLVertexArray::SetIndexBuffer(IndexBuffer* indexBuffer) {
		glBindVertexArray(m_RendererId);
		indexBuffer->Bind();
		m_IndexBuffer = indexBuffer;
	}

}
