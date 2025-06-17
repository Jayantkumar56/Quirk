

#include "Qkpch.h"
#include "Renderer.h"

#include "Core/Camera/Camera.h"

namespace Quirk {

	Renderer::Renderer(ConstView<RHI::RenderSystem> renderSystem) noexcept :
			m_RenderSystem  ( renderSystem               ),
			m_ShaderLibrary ( renderSystem->GetFactory() )
	{
		ConstView<RHI::Factory>       factory       = m_RenderSystem->GetFactory();
		ConstView<RHI::RenderCommand> renderCommand = m_RenderSystem->GetRenderCommandContext();

		renderCommand->SetClearColor({ 0.10156f, 0.17968f, 0.20703f, 1.0f });
		renderCommand->EnableBlending();
		renderCommand->EnableDepthTesting();
		//renderCommand->SetCullMode(RHI::CullMode::Back);

		m_SceneData.MeshShader = m_ShaderLibrary.LoadShader("assets/Shaders/Mesh.glsl");
		m_SceneData.LightSourceShader = m_ShaderLibrary.LoadShader("assets/Shaders/emissive_mesh.glsl");

		m_SceneData.MeshVertexArray = factory->CreateVertexArray();

		m_SceneData.MeshPositionBuffer = factory->CreateVertexBuffer();
		m_SceneData.MeshPositionBuffer->SetLayout({ { RHI::ShaderDataType::Float3,  "a_Position" } });
		m_SceneData.MeshVertexArray->AddVertexBuffer(m_SceneData.MeshPositionBuffer);

		m_SceneData.MeshNormalBuffer = factory->CreateVertexBuffer();
		m_SceneData.MeshNormalBuffer->SetLayout({ { RHI::ShaderDataType::Float3,  "a_Normal"   } });
		m_SceneData.MeshVertexArray->AddVertexBuffer(m_SceneData.MeshNormalBuffer);

		m_SceneData.MeshUVBuffer = factory->CreateVertexBuffer();
		m_SceneData.MeshUVBuffer->SetLayout({ { RHI::ShaderDataType::Float2,  "a_TexCoord" } });
		m_SceneData.MeshVertexArray->AddVertexBuffer(m_SceneData.MeshUVBuffer);
	}

	void Renderer::BeginScene(const glm::mat4& projectionView, glm::vec3& cameraPos) {
		m_SceneData.ProjectionViewMatrix = projectionView;
		m_SceneData.CameraPosition = cameraPos;
	}

	void Renderer::Submit(Entity entity, std::vector<Entity>& lightSources) {
		ConstView<RHI::RenderCommand> renderCommand = m_RenderSystem->GetRenderCommandContext();

		auto& transform = entity.GetComponent<TransformComponent>();
		auto& mesh      = entity.GetComponent<MeshRendererComponent>().MeshObject;
		int entityId    = (uint32_t)entity;

		if (!mesh.Data.Positions.size()) return;

		m_SceneData.MeshVertexArray->Bind();

		const void* data  = (const void*)mesh.Data.Positions.data();
		uint32_t dataSize = (uint32_t)mesh.Data.Positions.size() * sizeof(mesh.Data.Positions[0]);
		m_SceneData.MeshPositionBuffer->UploadData(data, dataSize);

		data     = (const void*)mesh.Data.Normals.data();
		dataSize = (uint32_t)mesh.Data.Normals.size() * sizeof(mesh.Data.Normals[0]);
		m_SceneData.MeshNormalBuffer->UploadData(data, dataSize);

		data     = (const void*)mesh.Data.UV.data();
		dataSize = (uint32_t)mesh.Data.Normals.size() * sizeof(mesh.Data.UV[0]);
		m_SceneData.MeshUVBuffer->UploadData(data, dataSize);

		glm::mat4 rotation = glm::toMat4(glm::quat(transform.Rotation));

		m_SceneData.MeshShader->Bind();
		m_SceneData.MeshShader->UploadUniform( "u_ViewProjection", m_SceneData.ProjectionViewMatrix );
		m_SceneData.MeshShader->UploadUniform( "u_CameraPosition", m_SceneData.CameraPosition       );
		m_SceneData.MeshShader->UploadUniform( "u_Transform",      transform.GetTransform()         );
		m_SceneData.MeshShader->UploadUniform( "u_Rotation",       rotation                         );
		m_SceneData.MeshShader->UploadUniform( "u_EntityId",       &entityId, 1                     );

		auto& material = entity.GetComponent<MeshRendererComponent>().MaterialProperties;
		m_SceneData.MeshShader->UploadUniform( "u_Material.Ambient",   material.Ambient       );
		m_SceneData.MeshShader->UploadUniform( "u_Material.Diffuse",   material.Diffuse       );
		m_SceneData.MeshShader->UploadUniform( "u_Material.Specular",  material.Specular      );
		m_SceneData.MeshShader->UploadUniform( "u_Material.Shininess", &material.Shininess, 1 );

		if (!lightSources.empty()) {
			auto& light    = lightSources[0].GetComponent<LightComponent>();
			auto& lightPos = lightSources[0].GetComponent<TransformComponent>().Translation;

			m_SceneData.MeshShader->UploadUniform( "u_Light.Position", lightPos           );
			m_SceneData.MeshShader->UploadUniform( "u_Light.Color",    light.Color        );
			m_SceneData.MeshShader->UploadUniform( "u_Light.Ambient",  &light.Ambient,  1 );
			m_SceneData.MeshShader->UploadUniform( "u_Light.Diffuse",  &light.Diffuse,  1 );
			m_SceneData.MeshShader->UploadUniform( "u_Light.Specular", &light.Specular, 1 );
		}
		else {
			glm::vec4 color { 1.0f, 1.0f, 1.0f, 1.0f };
			glm::vec3& lightPos = transform.Translation;

			m_SceneData.MeshShader->UploadUniform("u_Light.Color",    color   );
			m_SceneData.MeshShader->UploadUniform("u_Light.Position", lightPos);
		}

		uint32_t vertCount = 36;
		renderCommand->DrawVertices(vertCount);
	}

	void Renderer::SubmitLightSource(Entity entity) {
		ConstView<RHI::RenderCommand> renderCommand = m_RenderSystem->GetRenderCommandContext();

		if (!entity.HasComponent<MeshRendererComponent>()) return;

		auto transform = entity.GetComponent<TransformComponent>().GetTransform();
		auto& mesh     = entity.GetComponent<MeshRendererComponent>().MeshObject;
		auto& light    = entity.GetComponent<LightComponent>();
		int entityId   = (uint32_t)entity;

		if (!mesh.Data.Positions.size()) return;

		m_SceneData.MeshVertexArray->Bind();

		const void* data  = (const void*)mesh.Data.Positions.data();
		uint32_t dataSize = (uint32_t)mesh.Data.Positions.size() * sizeof(mesh.Data.Positions[0]);
		m_SceneData.MeshPositionBuffer->UploadData(data, dataSize);

		data     = (const void*)mesh.Data.Normals.data();
		dataSize = (uint32_t)mesh.Data.Normals.size() * sizeof(mesh.Data.Normals[0]);
		m_SceneData.MeshNormalBuffer->UploadData(data, dataSize);

		data     = (const void*)mesh.Data.UV.data();
		dataSize = (uint32_t)mesh.Data.UV.size() * sizeof(mesh.Data.UV[0]);
		m_SceneData.MeshUVBuffer->UploadData(data, dataSize);

		m_SceneData.LightSourceShader->Bind();
		m_SceneData.LightSourceShader->UploadUniform("u_ViewProjection", m_SceneData.ProjectionViewMatrix);
		m_SceneData.LightSourceShader->UploadUniform("u_Transform",      transform                       );
		m_SceneData.LightSourceShader->UploadUniform("u_EntityId",       &entityId,    1                 );
		m_SceneData.LightSourceShader->UploadUniform("u_Color",          light.Color                     );

		uint32_t vertCount = 36;
		renderCommand->DrawVertices(vertCount);
	}

	void Renderer::EndScene() {

	}

}

