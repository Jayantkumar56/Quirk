

#pragma once

#include "Core/RHI/IndexBuffer.h"


namespace Quirk::OpenGL {

	class IndexBuffer final : public RHI::IndexBuffer {
	public:
		IndexBuffer(uint32_t* indices, uint32_t count) noexcept;
		~IndexBuffer() noexcept;

		virtual void Bind()   const noexcept override;
		virtual void Unbind() const noexcept override;

	private:
		uint32_t m_RendererID;
	};

}
