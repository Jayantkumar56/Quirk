

#pragma once

#include "Core/Renderer/RendererPrimitives/Buffers.h"

namespace Quirk {

	class OpenGLVertexBuffer : public VertexBuffer {
	public:
		OpenGLVertexBuffer();
		OpenGLVertexBuffer(float* vertices, uint32_t size);
		OpenGLVertexBuffer(uint32_t size);
		virtual ~OpenGLVertexBuffer();

		virtual void UploadData(const void* data, uint32_t size, uint32_t offset = 0) override;

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual const BufferLayout& GetLayout() const override { return m_Layout; }
		virtual void SetLayout(const BufferLayout& layout) override { m_Layout = layout; }

		virtual uint32_t GetCount() const override { return m_Size / m_Layout.GetStride(); }
		virtual uint32_t GetSize() const override { return m_Size; }

	private:
		uint32_t m_RendererID;
		uint32_t m_Size;
		BufferLayout m_Layout;
	};

	class OpenGLIndexBuffer : public IndexBuffer {
	public:
		OpenGLIndexBuffer(uint32_t* indices, uint32_t count);
		virtual ~OpenGLIndexBuffer();

		virtual void Bind() const;
		virtual void Unbind() const;

		virtual uint32_t GetCount() const { return m_Count; }

	private:
		uint32_t m_RendererID;
		uint32_t m_Count;
	};

}
