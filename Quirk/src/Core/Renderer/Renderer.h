

#pragma once

#include "Core/Assets/RenderAssets/ShaderLibrary.h"
#include "Core/Assets/Geometry/Geometry.h"
#include "Core/Scene/Entity.h"
#include "Core/RHI/RenderSystem.h"


namespace Quirk {

	class Renderer {
	public:
		Renderer(ConstView<RHI::RenderSystem> renderSystem) noexcept;

		void BeginScene(const glm::mat4& projectionView, glm::vec3& cameraPos);
		void Submit(Entity entity, std::vector<Entity>& lightSources);
		void SubmitLightSource(Entity entity);
		void EndScene();

	private:
		struct SceneData {
			// camera data
			glm::mat4 ProjectionViewMatrix;
			glm::vec3 CameraPosition;

			// general mesh shader
			Ref<RHI::Shader>       MeshShader;
			Ref<RHI::VertexArray>  MeshVertexArray;
			Ref<RHI::VertexBuffer> MeshPositionBuffer;
			Ref<RHI::VertexBuffer> MeshNormalBuffer;
			Ref<RHI::VertexBuffer> MeshUVBuffer;

			// for light sources
			Ref<RHI::Shader> LightSourceShader;
		};

	private:
		ConstView<RHI::RenderSystem> m_RenderSystem;
		ShaderLibrary                m_ShaderLibrary;
		SceneData                    m_SceneData;
	};

}
