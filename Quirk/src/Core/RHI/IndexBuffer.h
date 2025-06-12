

#pragma once

#include "Core/Core.h"

#include <utility>


namespace Quirk::RHI {

	class IndexBuffer {
	public:
		virtual ~IndexBuffer() noexcept = default;

		virtual void Bind()   const noexcept = 0;
		virtual void Unbind() const noexcept = 0;

		inline uint32_t GetCount() const noexcept { return m_Count; }

	protected:
		uint32_t m_Count{};
	};

}
