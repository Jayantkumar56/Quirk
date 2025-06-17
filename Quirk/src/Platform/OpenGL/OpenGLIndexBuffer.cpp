

#include "Qkpch.h"

#include "OpenGLIndexBuffer.h"

#include "glad/glad.h"


namespace Quirk::OpenGL {

	IndexBuffer::IndexBuffer(uint32_t* indices, uint32_t count) noexcept {
		m_Count = count;

		glCreateBuffers(1, &m_RendererID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
	}

	IndexBuffer::~IndexBuffer() noexcept {
		glDeleteBuffers(1, &m_RendererID);
	}

	void Quirk::OpenGL::IndexBuffer::Bind() const noexcept {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
	}

	void IndexBuffer::Unbind() const noexcept {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

}
