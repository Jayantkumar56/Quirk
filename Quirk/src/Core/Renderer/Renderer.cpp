

#include "Qkpch.h"
#include "Renderer.h"
#include "RenderCommands.h"

#include "Core/Camera/PerspectiveCamera.h"

namespace Quirk {

	Renderer::SceneData Renderer::m_SceneData;

	void Renderer::InitRenderer(RendererAPI::API rendererAPI) {
		RenderCommands::Init(rendererAPI);
		RenderCommands::SetClearColor({ 0.10156f, 0.17968f, 0.20703f, 1.0f });
		RenderCommands::EnableFaceCulling();
	}

	void Renderer::BeginScene(OrthographicCamera& camera) {
		m_SceneData.ViewProjectionMatrix = camera.GetProjectionViewMatrix();
	}

	void Renderer::BeginScene(PerspectiveCamera& camera) {
		m_SceneData.ViewProjectionMatrix = camera.GetProjectionViewMatrix();
	}

	void Renderer::EndScene() {
	}

	void Renderer::Submit(Ref<Shader>& shader, Ref<VertexArray>& vertexArray) {
		shader->Bind();
		shader->UploadUniform("u_ViewProjection", m_SceneData.ViewProjectionMatrix);

		vertexArray->Bind();
		//RenderCommands::DrawIndexed(vertexArray);
		RenderCommands::DrawVertices(vertexArray);
	}

}

