

#pragma once

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Core/Utility/View.h"

#include <span>


namespace Quirk::RHI {

	class VertexArray {
	public:
		virtual ~VertexArray() = default;

		virtual void Bind()   const noexcept = 0;
		virtual void Unbind() const noexcept = 0;

		virtual void SetIndexBuffer(Ref<IndexBuffer>& indexBuffer)                            noexcept = 0;
		virtual void AddVertexBuffer(Ref<VertexBuffer>& vertexBuffer, bool instanced = false) noexcept = 0;

		inline ConstView<IndexBuffer>             GetIndexBuffer()   const noexcept { return m_IndexBuffer;   }
		inline std::span<const Ref<VertexBuffer>> GetVertexBuffers() const noexcept { return m_VertexBuffers; }

	protected:
		Ref<IndexBuffer>               m_IndexBuffer;
		std::vector<Ref<VertexBuffer>> m_VertexBuffers;
	};

}
