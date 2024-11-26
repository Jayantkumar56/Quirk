

#pragma once

#include "Core/Renderer/Buffers/FrameBuffer.h"
#include "glad/glad.h"

namespace Quirk {

	class OpenGLFrameBuffer: public FrameBuffer{
	public:
		OpenGLFrameBuffer(const FrameBufferSpecification& spec);
		~OpenGLFrameBuffer() { InvalidateAttachments(); glDeleteFramebuffers(1, &m_RendererID); }

		virtual inline void Bind()   override { glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID); }	
		virtual inline void Unbind() override { glBindFramebuffer(GL_FRAMEBUFFER, 0);			 }

		virtual void ClearAttachments() override;
		virtual void SetAttachments(std::initializer_list<FrameBufferAttachmentSpecification> attachmentsSpec) override;

		virtual inline uint32_t GetDepthStencilAttachment()      const override { return m_DepthStencilAttachment;  }
		virtual inline uint32_t GetColorAttachment(size_t index) const override { return m_ColorAttachments[index]; }

		virtual inline FrameBufferSpecification			  GetSpecification()				const override { return m_FrameBufferSpec; }
		virtual inline FrameBufferAttachmentSpecification GetDepthAttachmentSpecification() const override { return m_DepthAttachmentSpec; }
		virtual inline const std::vector<FrameBufferAttachmentSpecification>& GetColorAttachmentSpecification() const override { 
			return m_ColorAttachmentsSpec;
		}

		virtual void Resize(uint16_t width, uint16_t height) override;
		virtual void GetColorPixelData(size_t index, int x, int y, void* outputData, int dataCount) override;

	private:
		uint32_t CreateBuffer(GLint internalFormat, GLenum format, GLenum dataType, FrameBufferAttachmentSpecification spec) const;

		void CreateAttachments();
		void InvalidateAttachments() const;

	private:
		uint32_t m_RendererID;
		uint32_t m_DepthStencilAttachment = 0;
		std::vector<uint32_t> m_ColorAttachments;

		FrameBufferSpecification m_FrameBufferSpec;
		FrameBufferAttachmentSpecification m_DepthAttachmentSpec;
		std::vector<FrameBufferAttachmentSpecification> m_ColorAttachmentsSpec;
	};

}
