

#include "Qkpch.h"
#include "Renderer.h"
#include "RenderCommands.h"

#include "Core/Camera/Camera.h"

namespace Quirk {

	Renderer::SceneData Renderer::s_SceneData;

	void Renderer::InitRenderer() {
		RenderCommands::SetClearColor({ 0.10156f, 0.17968f, 0.20703f, 1.0f });
		RenderCommands::EnableBlending();

		s_SceneData.MeshShader      = ShaderLibrary::LoadShader("assets/Shaders/Mesh.glsl");
		s_SceneData.MeshVertexArray = VertexArray::Create();

		s_SceneData.MeshVertexBuffer = VertexBuffer::Create();
		s_SceneData.MeshVertexBuffer->SetLayout({
			{ ShaderDataType::Float3,  "a_Position" }
		});
		s_SceneData.MeshVertexArray->AddVertexBuffer(s_SceneData.MeshVertexBuffer);
	}

	void Renderer::BeginScene(const glm::mat4& projectionView) {
		s_SceneData.ProjectionViewMatrix = projectionView;
	}

	void Renderer::Submit(Entity entity) {
		auto transform = entity.GetComponent<TransformComponent>().GetTransform();
		auto& mesh     = entity.GetComponent<MeshComponent>().MeshObject;
		int entityId   = (uint32_t)entity;

		if (!mesh.Data.Positions.size())
			return;

		s_SceneData.MeshVertexArray->Bind();

		const void* data  = (const void*)mesh.Data.Positions.data();
		uint32_t dataSize = (uint32_t)mesh.Data.Positions.size() * sizeof(mesh.Data.Positions[0]);
		s_SceneData.MeshVertexBuffer->UploadData(data, dataSize);

		s_SceneData.MeshShader->Bind();
		s_SceneData.MeshShader->UploadUniform("u_ViewProjection", s_SceneData.ProjectionViewMatrix);
		s_SceneData.MeshShader->UploadUniform("u_Transform", transform);
		s_SceneData.MeshShader->UploadUniform("u_EntityId", &entityId, 1);

		uint32_t vertCount = (uint32_t)mesh.Data.Positions.size() / 3;
		RenderCommands::DrawVertices(vertCount);
	}

	void Renderer::EndScene() {

	}

}

