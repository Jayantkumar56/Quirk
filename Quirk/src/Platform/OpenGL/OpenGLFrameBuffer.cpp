
#include "Qkpch.h"
#include "Core/Core.h"
#include "OpenGLFrameBuffer.h"
#include "glad/glad.h"

namespace Quirk {

	OpenGLFrameBuffer::OpenGLFrameBuffer(const FrameBufferSpecification& spec):
			m_Specification(spec)
	{
		glGenFramebuffers(1, &m_RendererID);
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);

		glCreateTextures(GL_TEXTURE_2D, 1, &m_ColorBuffer);
		glBindTexture(GL_TEXTURE_2D, m_ColorBuffer);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, spec.Width, spec.Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorBuffer, 0);

		glCreateTextures(GL_TEXTURE_2D, 1, &m_DepthBuffer);
		glBindTexture(GL_TEXTURE_2D, m_DepthBuffer);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, spec.Width, spec.Height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_DepthBuffer, 0);

		//glGenRenderbuffers(1, &m_RenderBuffer);
		//glBindRenderbuffer(GL_RENDERBUFFER, m_RenderBuffer);
		//glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, spec.Width, spec.Height);
		//glBindRenderbuffer(GL_RENDERBUFFER, 0);
		//glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
		//	GL_RENDERBUFFER, m_RenderBuffer);

		QK_CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Uncomplete FrameBuffer!");
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	OpenGLFrameBuffer::~OpenGLFrameBuffer() {
		glDeleteFramebuffers(1, &m_RendererID);
	}

	void OpenGLFrameBuffer::Bind() {
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
	}

	void OpenGLFrameBuffer::Unbind() {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

}
