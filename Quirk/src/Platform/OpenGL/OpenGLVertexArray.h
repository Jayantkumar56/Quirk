

#pragma once

#include "Core/Renderer/VertexArray.h"

namespace Quirk {

	class OpenGLVertexArray : public VertexArray {
	public:
		OpenGLVertexArray();
		~OpenGLVertexArray();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void AddVertexBuffer(Ref<VertexBuffer>& vertexBuffer, bool instanced = false) override;
		virtual void AddInstancedVertexBuffer(Ref<VertexBuffer>& vertexBuffer) override;
		virtual void SetIndexBuffer(Ref<IndexBuffer>& indexBuffer) override;

		virtual const std::vector< Ref<VertexBuffer> >& GetVertexBuffers() const override { return m_VertexBuffers; }
		virtual const Ref<IndexBuffer>& GetIndexBuffer() const override { return m_IndexBuffer; }

	private:
		uint32_t m_RendererId;
		uint32_t m_Index = 0;
		std::vector<Ref<VertexBuffer>> m_VertexBuffers;
		Ref<IndexBuffer> m_IndexBuffer;
	};

}
