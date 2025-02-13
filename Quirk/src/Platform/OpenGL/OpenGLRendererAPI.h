

#pragma once

#include "Core/Renderer/RendererAPI.h"
#include "Platform/OpenGL/OpenGLContext.h"


namespace Quirk {

	class OpenGLRendererAPI : public RendererAPI {
	public:
		OpenGLRendererAPI();

		virtual inline void SetClearColor(const glm::vec4& color) const override;
		virtual inline void Clear() const override;
		virtual inline void UpdateViewPort(uint32_t width, uint32_t height) const override;

		virtual inline void EnableBlending() const override;

		virtual inline void EnableFaceCulling() const override;
		virtual inline void DisableFaceCulling() const override;

		virtual inline void DrawIndexed(Ref<VertexArray>& vertexArray, uint32_t indicesCount) override;
		virtual inline void DrawIndexedInstanced(Ref<VertexArray>& vertexArray, uint32_t noOfInstances) override;
		virtual inline void DrawVertices(uint32_t vertexCount) override;
	};

}

