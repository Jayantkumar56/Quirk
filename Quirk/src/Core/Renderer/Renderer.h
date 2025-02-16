

#pragma once

#include "Core/Core.h"

#include "RendererAPI.h"
#include "Core/Renderer/RendererPrimitives/VertexArray.h"
#include "Core/Renderer/RendererPrimitives/Shader.h"
#include "Texture.h"

#include "Geometry/Geometry.h"
#include "Core/Scene/Entity.h"

namespace Quirk {

	class Renderer {
	public:
		static void InitRenderer();

		static void BeginScene(const glm::mat4& projectionView, glm::vec3& cameraPos);
		static void Submit(Entity entity, std::vector<Entity>& lightSources);
		static void SubmitLightSource(Entity entity);
		static void EndScene();

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

	private:
		struct SceneData {
			// camera data
			glm::mat4 ProjectionViewMatrix;
			glm::vec3 CameraPosition;

			// general mesh shader
			Ref<Shader>       MeshShader;
			Ref<VertexArray>  MeshVertexArray;
			Ref<VertexBuffer> MeshVertexBuffer;

			// for light sources
			Ref<Shader> LightSourceShader;
		};

		static SceneData s_SceneData;
	};

}
