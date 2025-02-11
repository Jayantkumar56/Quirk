

#pragma once

#include "Core/Core.h"

#include "RendererAPI.h"
#include "Core/Renderer/RendererPrimitives/VertexArray.h"
#include "Core/Renderer/RendererPrimitives/Shader.h"
#include "Texture.h"

namespace Quirk {

	class Renderer {
	public:
		static void InitRenderer(RendererAPI::API rendererAPI);

		static void BeginScene(const glm::mat4& projectionView);
		static void EndScene();

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

	private:
		struct SceneData {
			// camera data
			glm::mat4 ProjectionViewMatrix;
		};

		static SceneData m_SceneData;
	};

}
