

#include "Qkpch.h"
#include "Renderer.h"
#include "RenderCommands.h"

#include "Core/Camera/Camera.h"

namespace Quirk {

	Renderer::SceneData Renderer::s_SceneData;

	void Renderer::InitRenderer() {
		RenderCommands::SetClearColor({ 0.10156f, 0.17968f, 0.20703f, 1.0f });
		RenderCommands::EnableBlending();

		s_SceneData.MeshShader        = ShaderLibrary::LoadShader("assets/Shaders/Mesh.glsl");
		s_SceneData.LightSourceShader = ShaderLibrary::LoadShader("assets/Shaders/emissive_mesh.glsl");

		s_SceneData.MeshVertexArray = VertexArray::Create();

		s_SceneData.MeshVertexBuffer = VertexBuffer::Create();
		s_SceneData.MeshVertexBuffer->SetLayout({
			{ ShaderDataType::Float3,  "a_Position" },
			{ ShaderDataType::Float3,  "a_Normal"   },
			{ ShaderDataType::Float2,  "a_TexCoord" }
		});
		s_SceneData.MeshVertexArray->AddVertexBuffer(s_SceneData.MeshVertexBuffer);
	}

	void Renderer::BeginScene(const glm::mat4& projectionView, glm::vec3& cameraPos) {
		s_SceneData.ProjectionViewMatrix = projectionView;
		s_SceneData.CameraPosition = cameraPos;
	}

	void Renderer::Submit(Entity entity, std::vector<Entity>& lightSources) {
		auto& transform = entity.GetComponent<TransformComponent>();
		auto& mesh      = entity.GetComponent<MeshComponent>().MeshObject;
		int entityId    = (uint32_t)entity;

		if (!mesh.Data.Positions.size()) return;

		s_SceneData.MeshVertexArray->Bind();

		const void* data  = (const void*)mesh.Data.Positions.data();
		uint32_t dataSize = (uint32_t)mesh.Data.Positions.size() * sizeof(mesh.Data.Positions[0]);
		s_SceneData.MeshVertexBuffer->UploadData(data, dataSize);

		glm::mat4 rotation = glm::toMat4(glm::quat(transform.Rotation));

		s_SceneData.MeshShader->Bind();
		s_SceneData.MeshShader->UploadUniform( "u_ViewProjection", s_SceneData.ProjectionViewMatrix );
		s_SceneData.MeshShader->UploadUniform( "u_CameraPosition", s_SceneData.CameraPosition       );
		s_SceneData.MeshShader->UploadUniform( "u_Transform",      transform.GetTransform()         );
		s_SceneData.MeshShader->UploadUniform( "u_Rotation",       rotation                         );
		s_SceneData.MeshShader->UploadUniform( "u_EntityId",       &entityId, 1                     );

		auto& material = entity.GetComponent<MeshComponent>().MaterialProperties;
		s_SceneData.MeshShader->UploadUniform( "u_Material.Ambient",   material.Ambient       );
		s_SceneData.MeshShader->UploadUniform( "u_Material.Diffuse",   material.Diffuse       );
		s_SceneData.MeshShader->UploadUniform( "u_Material.Specular",  material.Specular      );
		s_SceneData.MeshShader->UploadUniform( "u_Material.Shininess", &material.Shininess, 1 );

		if (!lightSources.empty()) {
			auto& light    = lightSources[0].GetComponent<LightComponent>();
			auto& lightPos = lightSources[0].GetComponent<TransformComponent>().Translation;

			s_SceneData.MeshShader->UploadUniform( "u_Light.Position", lightPos           );
			s_SceneData.MeshShader->UploadUniform( "u_Light.Color",    light.Color        );
			s_SceneData.MeshShader->UploadUniform( "u_Light.Ambient",  &light.Ambient,  1 );
			s_SceneData.MeshShader->UploadUniform( "u_Light.Diffuse",  &light.Diffuse,  1 );
			s_SceneData.MeshShader->UploadUniform( "u_Light.Specular", &light.Specular, 1 );
		}
		else {
			glm::vec4 color { 1.0f, 1.0f, 1.0f, 1.0f };
			glm::vec3& lightPos = transform.Translation;

			s_SceneData.MeshShader->UploadUniform("u_Light.Color",    color   );
			s_SceneData.MeshShader->UploadUniform("u_Light.Position", lightPos);
		}

		uint32_t vertCount = 36;
		RenderCommands::DrawVertices(vertCount);
	}

	void Renderer::SubmitLightSource(Entity entity) {
		if (!entity.HasComponent<MeshComponent>()) return;

		auto transform = entity.GetComponent<TransformComponent>().GetTransform();
		auto& mesh     = entity.GetComponent<MeshComponent>().MeshObject;
		auto& light    = entity.GetComponent<LightComponent>();
		int entityId   = (uint32_t)entity;

		if (!mesh.Data.Positions.size()) return;

		s_SceneData.MeshVertexArray->Bind();

		const void* data  = (const void*)mesh.Data.Positions.data();
		uint32_t dataSize = (uint32_t)mesh.Data.Positions.size() * sizeof(mesh.Data.Positions[0]);
		s_SceneData.MeshVertexBuffer->UploadData(data, dataSize);

		s_SceneData.LightSourceShader->Bind();
		s_SceneData.LightSourceShader->UploadUniform("u_ViewProjection", s_SceneData.ProjectionViewMatrix);
		s_SceneData.LightSourceShader->UploadUniform("u_Transform",      transform                       );
		s_SceneData.LightSourceShader->UploadUniform("u_EntityId",       &entityId,    1                 );
		s_SceneData.LightSourceShader->UploadUniform("u_Color",          light.Color                     );

		uint32_t vertCount = 36;
		RenderCommands::DrawVertices(vertCount);
	}

	void Renderer::EndScene() {

	}

}

