

#include "Qkpch.h"

#include "glad/glad.h"
#include "OpenGLRendererAPI.h"

namespace Quirk {

	OpenGLRendererAPI::OpenGLRendererAPI() : 
			RendererAPI(RendererAPI::API::OpenGL)
	{
	}

	inline void OpenGLRendererAPI::SetClearColor(const glm::vec4& color) const {
		glClearColor(color.r, color.g, color.b, color.a);
	}

	inline void OpenGLRendererAPI::EnableFaceCulling() const {
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glFrontFace(GL_CCW);
	}

	inline void OpenGLRendererAPI::DisableFaceCulling() const {
		glDisable(GL_CULL_FACE);
	}

	inline void OpenGLRendererAPI::Clear() const {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	inline void OpenGLRendererAPI::UpdateViewPort(uint32_t width, uint32_t height) const {
		glViewport(0, 0, width, height);
	}

	inline void OpenGLRendererAPI::EnableBlending() const {
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnable(GL_DEPTH_TEST);
	}

	inline void OpenGLRendererAPI::DrawIndexed(Ref<VertexArray>& vertexArray, uint32_t indicesCount) {
		glDrawElements(GL_TRIANGLES, indicesCount, GL_UNSIGNED_INT, nullptr);
		glFlush();
	}

	inline void OpenGLRendererAPI::DrawIndexedInstanced(Ref<VertexArray>& vertexArray, uint32_t noOfInstances) {
		glDrawElementsInstanced(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr, noOfInstances);
		glFlush();
	}

	inline void OpenGLRendererAPI::DrawVertices(Ref<VertexArray>& vertexArray) {
		glDrawArrays(GL_TRIANGLES, 0, vertexArray->GetVertexBuffers()[0]->GetCount());
		glFlush();
	}

}

