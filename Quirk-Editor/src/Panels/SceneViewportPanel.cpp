


#include "SceneViewportPanel.h"

namespace Quirk {

	SceneViewportPanel::SceneViewportPanel(const Ref<Scene>& scene, uint16_t width, uint16_t height) :
			m_PanelWidth	  (width),
			m_PanelHeight	  (height),
			m_Frame			  (FrameBuffer::Create({ m_PanelWidth, m_PanelHeight })),
			m_RendererStats	  ({ 0, 0 }),
			m_MainScene		  (scene),
			m_IsInFocus		  (false)
	{
		RenderCommands::UpdateViewPort(m_PanelWidth, m_PanelHeight);
	}

	bool SceneViewportPanel::OnEvent(Event& event) {
		return false;
	}

	void SceneViewportPanel::OnUpdate() {
		if (!m_IsInFocus) {
			return;
		}

		m_MainScene->OnUpdate();
	}

	void SceneViewportPanel::OnImguiUiUpdate() {
		ImGuiIO& io = ImGui::GetIO();
		ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoTitleBar;

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("Scene Viewport");

		m_IsInFocus = ImGui::IsWindowFocused();
		CheckAndHandleResize();
		RenderViewport();

		uint32_t color = m_Frame->GetColorBuffer();
		ImGui::Image((void*)color, ImVec2(m_PanelWidth, m_PanelHeight), { 0, 1 }, { 1, 0 });

		ImGui::End();
		ImGui::PopStyleVar();
	}

	void SceneViewportPanel::CheckAndHandleResize() {
		ImVec2 windowSize = ImGui::GetContentRegionAvail();

		if (windowSize.x < 0 || windowSize.y < 0) {
			return;
		}

		if (m_PanelWidth != (int)windowSize.x || m_PanelHeight != (int)windowSize.y) {
			m_PanelWidth = (int)windowSize.x;
			m_PanelHeight = (int)windowSize.y;

			m_Frame->Resize(m_PanelWidth, m_PanelHeight);
			RenderCommands::UpdateViewPort(m_PanelWidth, m_PanelHeight);
			m_MainScene->OnViewportResize(m_PanelWidth, m_PanelHeight);
		}
	}

	void SceneViewportPanel::RenderViewport() {
		m_Frame->Bind();
		RenderCommands::Clear();
		Renderer2D::ResetStats();
		
		m_MainScene->RenderScene();

		m_Frame->Unbind();
		m_RendererStats = Renderer2D::GetStats();
	}

}

