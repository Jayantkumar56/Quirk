

#include "Qkpch.h"
#include "OpenGLVertexArray.h"
#include "glad/glad.h"

namespace Quirk {

	OpenGLVertexArray::OpenGLVertexArray() {
		m_IndexBuffer = nullptr;
		glCreateVertexArrays(1, &m_RendererId);
	}

	OpenGLVertexArray::~OpenGLVertexArray() {
		glDeleteVertexArrays(1, &m_RendererId);
	}

	void OpenGLVertexArray::Bind() const {
		glBindVertexArray(m_RendererId);
	}

	void OpenGLVertexArray::Unbind() const {
		glBindVertexArray(0);
	}

	void OpenGLVertexArray::AddVertexBuffer(Ref<VertexBuffer>& vertexBuffer, bool instanced) {
		QK_CORE_ASSERT(vertexBuffer->GetLayout().GetElements().size(), "Vertex Buffer has no layout!");

		glBindVertexArray(m_RendererId);
		vertexBuffer->Bind();

		const auto& layout = vertexBuffer->GetLayout();
		uint64_t offset    = 0;

		for (const auto& element : layout) {
			if (element.IsIntType()) {
				glEnableVertexAttribArray(m_Index);
				glVertexAttribIPointer(m_Index, GetComponentCount(element.Type), GL_INT, layout.GetStride(), (const void*)offset);

				if (instanced)
					glVertexAttribDivisor(m_Index, 1);

				++m_Index;
			}
			else if (element.Type == ShaderDataType::Mat4) {
				uint64_t elementOffset = offset;
				for (int i = 0; i < 4; ++i, elementOffset += static_cast<uint64_t>(4 * 4)) {
					glEnableVertexAttribArray(m_Index);
					glVertexAttribPointer(m_Index, 4, GL_FLOAT, GL_FALSE, layout.GetStride(), (const void*)elementOffset);

					if (instanced)
						glVertexAttribDivisor(m_Index, 1);

					++m_Index;
				}
			}
			else if (element.Type == ShaderDataType::Mat3) {
				uint64_t elementOffset = offset;
				for (int i = 0; i < 3; ++i, elementOffset += static_cast<uint64_t>(4 * 3)) {
					glEnableVertexAttribArray(m_Index);
					glVertexAttribPointer(m_Index, 3, GL_FLOAT, GL_FALSE, layout.GetStride(), (const void*)elementOffset);

					if (instanced)
						glVertexAttribDivisor(m_Index, 1);

					++m_Index;
				}
			}
			else {
				glEnableVertexAttribArray(m_Index);
				glVertexAttribPointer(m_Index, GetComponentCount(element.Type), GL_FLOAT, GL_FALSE, layout.GetStride(), (const void*)offset);

				if (instanced)
					glVertexAttribDivisor(m_Index, 1);

				++m_Index;
			}

			offset += ShaderDataTypeSize(element.Type);
		}

		m_VertexBuffers.push_back(vertexBuffer);
	}

	void OpenGLVertexArray::AddInstancedVertexBuffer(Ref<VertexBuffer>& vertexBuffer) {
		AddVertexBuffer(vertexBuffer, true);
	}

	void OpenGLVertexArray::SetIndexBuffer(Ref<IndexBuffer>& indexBuffer) {
		glBindVertexArray(m_RendererId);
		indexBuffer->Bind();
		m_IndexBuffer = indexBuffer;
	}

}
