

#include "Qkpch.h"
#include "Renderer.h"
#include "RenderCommands.h"

namespace Quirk {

	Renderer::SceneData Renderer::m_SceneData;

	void Renderer::InitRenderer(RendererAPI::API rendererAPI) {
		RenderCommands::Init(rendererAPI);
	}

	void Renderer::BeginScene(OrthographicCamera& camera) {
		m_SceneData.ViewProjectionMatrix = camera.GetProjectionViewMatrix();
	}

	void Renderer::EndScene() {
	}

	void Renderer::Submit(Ref<Shader>& shader, Ref<VertexArray>& vertexArray) {
		shader->Bind();
		shader->UploadUniform("u_ViewProjection", m_SceneData.ViewProjectionMatrix);

		vertexArray->Bind();
		RenderCommands::DrawIndexed(vertexArray);
	}

}

