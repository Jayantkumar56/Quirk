

#pragma once

#include "Core/RHI/VertexArray.h"


namespace Quirk::OpenGL {

	class VertexArray final : public RHI::VertexArray {
	public:
		VertexArray()  noexcept;
		~VertexArray() noexcept;

		virtual void Bind()   const noexcept override;
		virtual void Unbind() const noexcept override;

		virtual void SetIndexBuffer(Ref<RHI::IndexBuffer>& indexBuffer)                            noexcept override;
		virtual void AddVertexBuffer(Ref<RHI::VertexBuffer>& vertexBuffer, bool instanced = false) noexcept override;

	private:
		uint32_t m_Index = 0;
		uint32_t m_RendererId;
	};

}
