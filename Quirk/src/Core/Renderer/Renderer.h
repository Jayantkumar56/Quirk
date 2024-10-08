

#pragma once

#include "Core/Core.h"

#include "RendererAPI.h"
#include "VertexArray.h"
#include "Shader.h"

#include "Core/Camera/Camera.h"

namespace Quirk {

	class Renderer {
	public:
		static void InitRenderer(RendererAPI::API rendererAPI);

		static void BeginScene(const glm::mat4& projectionView);

		static void EndScene();
		static void Submit(Ref<Shader>& shader, Ref<VertexArray>& vertexArray);

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

	private:
		struct SceneData {
			glm::mat4 ViewProjectionMatrix;
		};

		static SceneData m_SceneData;
	};

}
