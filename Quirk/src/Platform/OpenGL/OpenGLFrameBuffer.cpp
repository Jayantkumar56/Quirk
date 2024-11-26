
#include "Qkpch.h"
#include "Core/Core.h"
#include "OpenGLFrameBuffer.h"
#include "OpenGLTexture.h"

namespace Quirk {

	OpenGLFrameBuffer::OpenGLFrameBuffer(const FrameBufferSpecification& spec) : m_FrameBufferSpec(spec) {
		glGenFramebuffers(1, &m_RendererID);
	}

	void OpenGLFrameBuffer::ClearAttachments() {
		if (m_DepthStencilAttachment)
			glClear(GL_DEPTH_BUFFER_BIT);

		for (int i = 0; i < (int)m_ColorAttachments.size(); ++i) {
			switch (m_ColorAttachmentsSpec[i].Type) {
				case FrameBufferTextureType::RGBA_8: {
					glClearBufferfv(GL_COLOR, i, m_ColorAttachmentsSpec[i].ClearData.RGBA); break;
				}

				case FrameBufferTextureType::RGB_8: {
					glClearBufferfv(GL_COLOR, i, m_ColorAttachmentsSpec[i].ClearData.RGBA); break;
				}

				case FrameBufferTextureType::RED_INTEGER: {
					glClearBufferiv(GL_COLOR, i, &(m_ColorAttachmentsSpec[i].ClearData.RedInteger)); break;
				}
			}
		}
	}

	void OpenGLFrameBuffer::SetAttachments(std::initializer_list<FrameBufferAttachmentSpecification> attachmentsSpec) {
		int colorAttachmentCount = 0;

		for (auto attachmentSpec : attachmentsSpec) {
			if (FrameBuffer::IsColorAttachment(attachmentSpec.Type)) {
				++colorAttachmentCount;
				m_ColorAttachmentsSpec.emplace_back(attachmentSpec);
			}
			else if (FrameBuffer::IsDepthAttachment(attachmentSpec.Type)) {
				QK_CORE_ASSERT(m_DepthAttachmentSpec.Type == FrameBufferTextureType::None, "Framebuffer do not support multiple DepthAttachments");
				m_DepthAttachmentSpec = attachmentSpec;
			}
		}

		QK_CORE_ASSERT(colorAttachmentCount < 32, "Provided ColorAttachments more than max limit of 32 attachments");
		m_ColorAttachments = std::vector<uint32_t>(colorAttachmentCount, 0);
		CreateAttachments();
	}

    void OpenGLFrameBuffer::Resize(uint16_t width, uint16_t height) {
		m_FrameBufferSpec.Width  = width;
		m_FrameBufferSpec.Height = height;

		InvalidateAttachments();
		CreateAttachments();
    }

	void OpenGLFrameBuffer::GetColorPixelData(size_t index, int x, int y, void* outputData, int dataCount) {
		QK_CORE_ASSERT(index < m_ColorAttachments.size(), "index exceeds the color attachment's count");
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
		glReadBuffer(GL_COLOR_ATTACHMENT0 + (GLenum)index);

		switch (m_ColorAttachmentsSpec[index].Type) {
			case FrameBufferTextureType::RED_INTEGER: {
				QK_CORE_ASSERT(dataCount == 1, "Can output only a single value from requested buffer");
				glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, outputData);
				break;
			}
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	uint32_t OpenGLFrameBuffer::CreateBuffer(GLint internalFormat, GLenum format, GLenum dataType, FrameBufferAttachmentSpecification spec) const {
		uint32_t buffer;

		glCreateTextures(GL_TEXTURE_2D, 1, &buffer);
		glBindTexture(GL_TEXTURE_2D, buffer);
		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, m_FrameBufferSpec.Width, m_FrameBufferSpec.Height, 0, format, dataType, NULL);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, TexutureFilterToOpenglFilter(spec.MinificationFilter));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, TexutureFilterToOpenglFilter(spec.MagficationFilter));

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, TextureWrapToOpenglWrap(spec.WrapMode));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, TextureWrapToOpenglWrap(spec.WrapMode));

		return buffer;
	}

	void OpenGLFrameBuffer::CreateAttachments() {
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
		GLenum colorAttachments[32] = { GL_COLOR_ATTACHMENT0 };
		GLint colorBufferCount = 0;

		for (size_t i = 0; i < m_ColorAttachmentsSpec.size(); ++i) {
			colorAttachments[colorBufferCount]   = GL_COLOR_ATTACHMENT0 + (GLenum)colorBufferCount;

			switch (m_ColorAttachmentsSpec[i].Type) {
				case FrameBufferTextureType::RGBA_8: {
					m_ColorAttachments[colorBufferCount] = CreateBuffer(GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE, m_ColorAttachmentsSpec[i]);
					break;
				}

				case FrameBufferTextureType::RGB_8: {
					m_ColorAttachments[colorBufferCount] = CreateBuffer(GL_RGB8, GL_RGB, GL_UNSIGNED_BYTE, m_ColorAttachmentsSpec[i]);
					break;
				}

				case FrameBufferTextureType::RED_INTEGER: {
					m_ColorAttachments[colorBufferCount] = CreateBuffer(GL_R32I, GL_RED_INTEGER, GL_INT, m_ColorAttachmentsSpec[i]);
					break;
				}
			}

			glFramebufferTexture2D(GL_FRAMEBUFFER, colorAttachments[colorBufferCount], GL_TEXTURE_2D, m_ColorAttachments[colorBufferCount], 0);
			++colorBufferCount;
		}

		switch (m_DepthAttachmentSpec.Type) {
			case FrameBufferTextureType::DEPTH_24_STENCIL_8: {
				m_DepthStencilAttachment = CreateBuffer(GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, m_DepthAttachmentSpec);
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_DepthStencilAttachment, 0);
				break;
			}
		}

		glDrawBuffers(colorBufferCount, colorAttachments);
		QK_CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Uncomplete FrameBuffer!");
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLFrameBuffer::InvalidateAttachments() const {
		glDeleteTextures((int)m_ColorAttachments.size(), m_ColorAttachments.data());

		if (m_DepthStencilAttachment)
			glDeleteTextures(1, &m_DepthStencilAttachment);
	}

}
