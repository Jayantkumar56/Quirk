

#pragma once

#include "Core/Renderer/RendererAPI.h"
#include "Platform/OpenGL/OpenGLContext.h"


namespace Quirk {

	class OpenGLRendererAPI : public RendererAPI {
	public:
		OpenGLRendererAPI();

		virtual inline void SetClearColor(const glm::vec4& color) const override;
		virtual inline void Clear() const override;
		virtual inline void SwapBuffers() override;

		virtual inline void EnableFaceCulling() const override;
		virtual inline void DisableFaceCulling() const override;

		virtual inline void DrawIndexed(Ref<VertexArray>& vertexArray) override;
		virtual inline void DrawVertices(Ref<VertexArray>& vertexArray) override;
		
	private:
		OpenGLContext m_Context;
	};

}

