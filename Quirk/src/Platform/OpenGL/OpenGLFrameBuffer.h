

#pragma once

#include "Core/Renderer/FrameBuffer.h"

namespace Quirk {

	class OpenGLFrameBuffer: public FrameBuffer{
	public:
		OpenGLFrameBuffer(const FrameBufferSpecification& spec);
		~OpenGLFrameBuffer();

		virtual void Bind() override;
		virtual void Unbind() override;

		virtual inline const uint32_t GetColorBuffer()					  const { return m_ColorBuffer; }
		virtual inline const uint32_t GetRenderBufferObject()			  const { return m_RenderBuffer; }
		virtual inline const FrameBufferSpecification& GetSpecification() const { return m_Specification; }

		virtual void Resize(uint16_t width, uint16_t height);

	private:
		void CreateBuffers();
		void InvalidateBuffers() const;

	private:
		uint32_t m_RendererID;
		uint32_t m_ColorBuffer, m_RenderBuffer, m_DepthBuffer;
		FrameBufferSpecification m_Specification;
	};

}
