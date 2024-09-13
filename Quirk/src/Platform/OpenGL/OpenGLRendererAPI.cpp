

#include "Qkpch.h"

#include "glad/glad.h"
#include "OpenGLRendererAPI.h"

namespace Quirk {

	OpenGLRendererAPI::OpenGLRendererAPI() : 
			RendererAPI(&m_Context, RendererAPI::API::OpenGL)
	{
		m_Context.Init();

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnable(GL_DEPTH_TEST);
	}

	inline void Quirk::OpenGLRendererAPI::SetClearColor(const glm::vec4& color) const {
		glClearColor(color.r, color.g, color.b, color.a);
	}

	inline void Quirk::OpenGLRendererAPI::EnableFaceCulling() const {
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glFrontFace(GL_CCW);
	}

	inline void Quirk::OpenGLRendererAPI::DisableFaceCulling() const {
		glDisable(GL_CULL_FACE);
	}

	inline void Quirk::OpenGLRendererAPI::Clear() const {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	inline void OpenGLRendererAPI::SwapBuffers() {
		m_Context.SwapBuffer();
	}

	inline void OpenGLRendererAPI::DrawIndexed(Ref<VertexArray>& vertexArray) {
		glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
		glFlush();
	}

	inline void OpenGLRendererAPI::DrawVertices(Ref<VertexArray>& vertexArray) {
		glDrawArrays(GL_TRIANGLES, 0, vertexArray->GetVertexBuffers()[0]->GetCount());
		glFlush();
	}

}

