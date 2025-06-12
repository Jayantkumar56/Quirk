

#include "Qkpch.h"

#include "VertexBuffer.h"

#include "glad/glad.h"


namespace Quirk::OpenGL {

	VertexBuffer::VertexBuffer() noexcept {
		m_Size = 0;
		glCreateBuffers(1, &m_RendererID);
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
	}

	VertexBuffer::VertexBuffer(BufferView buffer) noexcept {
		m_Size = static_cast<uint32_t>(buffer.Size());

		glCreateBuffers(1, &m_RendererID);
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
		glBufferData(GL_ARRAY_BUFFER, m_Size, static_cast<const void*>(buffer.Get()), GL_STATIC_DRAW);
	}

	VertexBuffer::VertexBuffer(uint32_t size) noexcept {
		m_Size = size;

		glCreateBuffers(1, &m_RendererID);
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
		glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
	}

	VertexBuffer::~VertexBuffer() noexcept {
		glDeleteBuffers(1, &m_RendererID);
	}

	void VertexBuffer::UploadData(const void* data, uint32_t size, uint32_t offset) noexcept {
		m_Size = size;
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
		glBufferData(GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW);
	}

	void VertexBuffer::Bind() const noexcept {
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
	}

	void VertexBuffer::Unbind() const noexcept {
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

}
