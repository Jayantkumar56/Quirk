

#include "Qkpch.h"

#include "glad/glad.h"
#include "OpenGLRendererAPI.h"

namespace Quirk {

	OpenGLRendererAPI::OpenGLRendererAPI() : 
			RendererAPI(&m_Context, RendererAPI::API::OpenGL)
	{
		m_Context.Init();
	}

	inline void Quirk::OpenGLRendererAPI::SetClearColor(const glm::vec4& color) const {
		glClearColor(color.r, color.g, color.b, color.a);
	}

	inline void Quirk::OpenGLRendererAPI::Clear() const {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	inline void OpenGLRendererAPI::SwapBuffers() {
		m_Context.SwapBuffer();
	}

}

