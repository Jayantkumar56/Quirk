

#include "Qkpch.h"
#include "Renderer.h"
#include "RenderCommands.h"

#include "Core/Camera/Camera.h"

namespace Quirk {

	Renderer::SceneData Renderer::m_SceneData;

	void Renderer::InitRenderer(RendererAPI::API rendererAPI) {
		RenderCommands::SetClearColor({ 0.10156f, 0.17968f, 0.20703f, 1.0f });
		RenderCommands::EnableBlending();
	}

	void Renderer::BeginScene(const glm::mat4& projectionView) {
		m_SceneData.ProjectionViewMatrix = projectionView;
	}

	void Renderer::EndScene() {

	}

}

