
#include "qkpch.h"
#include "OpenGLBuffers.h"

#include <glad/glad.h>

namespace Quirk {

	// VertexBuffer /////////////////////////////////////////////////////////////

	OpenGLVertexBuffer::OpenGLVertexBuffer() {
		m_Size = 0;
		glCreateBuffers(1, &m_RendererID);
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
	}

	OpenGLVertexBuffer::OpenGLVertexBuffer(float* vertices, uint32_t size) :
			m_Size(size)
	{
		glCreateBuffers(1, &m_RendererID);
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
		glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
	}

    OpenGLVertexBuffer::OpenGLVertexBuffer(uint32_t size) :
			m_Size(size)
	{
		glCreateBuffers(1, &m_RendererID);
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
		glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
    }

	OpenGLVertexBuffer::~OpenGLVertexBuffer() {
		glDeleteBuffers(1, &m_RendererID);
	}

    void OpenGLVertexBuffer::UploadData(const void* data, uint32_t size, uint32_t offset) {
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);

		if (size > m_Size)
			glBufferData(GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW);
		else {
			m_Size = size;
			glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
		}
    }

	void OpenGLVertexBuffer::Bind() const {
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
	}

	void OpenGLVertexBuffer::Unbind() const {
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	// IndexBuffer //////////////////////////////////////////////////////////////

	OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* indices, uint32_t count)
		: m_Count(count)
	{
		glCreateBuffers(1, &m_RendererID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
	}

	OpenGLIndexBuffer::~OpenGLIndexBuffer() {
		glDeleteBuffers(1, &m_RendererID);
	}

	void OpenGLIndexBuffer::Bind() const {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
	}

	void OpenGLIndexBuffer::Unbind() const {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

}
