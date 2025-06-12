

#pragma once

#include "Core/RHI/VertexBuffer.h"
#include "Core/Utility/Buffer.h"


namespace Quirk::OpenGL {

	class VertexBuffer final : public RHI::VertexBuffer {
	public:
		VertexBuffer()                  noexcept;
		VertexBuffer(BufferView buffer) noexcept;
		VertexBuffer(uint32_t size)     noexcept;
		~VertexBuffer()                 noexcept;

		virtual void UploadData(const void* data, uint32_t size, uint32_t offset = 0) noexcept override;

		virtual void Bind()   const noexcept override;
		virtual void Unbind() const noexcept override;

	private:
		uint32_t m_RendererID;
	};

}
