

#pragma once

#include "Core/Core.h"

namespace Quirk {

	struct FrameBufferSpecification {
		uint32_t Width = 0, Height = 0;
		bool SwapChainTarget = false;
	};

	class FrameBuffer {
	public:
		static Ref<FrameBuffer> Create(const FrameBufferSpecification& spec);

		virtual ~FrameBuffer() = default;

		virtual void Bind()	  = 0;
		virtual void Unbind() = 0;

		virtual inline const uint32_t GetColorBuffer()						const = 0;
		virtual inline const uint32_t GetRenderBufferObject()				const = 0;
		virtual inline const FrameBufferSpecification& GetSpecification()	const = 0;
		virtual void Resize(uint16_t width, uint16_t height)					  = 0;
	};

}
