


#include "SceneViewportPanel.h"
#include "FontManager.h"

namespace Quirk {

	SceneViewportPanel::SceneViewportPanel(uint16_t width, uint16_t height) :
			m_PanelWidth	  (width),
			m_PanelHeight	  (height),
			m_Frame			  (FrameBuffer::Create({ m_PanelWidth, m_PanelHeight })),
			m_RendererStats	  ({ 0, 0 }),
			m_IsInFocus		  (false)
	{
		RenderCommands::UpdateViewPort(m_PanelWidth, m_PanelHeight);
	}

	bool SceneViewportPanel::OnEvent(Event& event) {
		return false;
	}

	void SceneViewportPanel::OnUpdate(const Ref<Scene>& scene) {
		if (!m_IsInFocus) {
			return;
		}

		scene->OnUpdate();
	}

	void SceneViewportPanel::OnImguiUiUpdate(const Ref<Scene>& scene) {
		ImGuiIO& io = ImGui::GetIO();
		ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoTitleBar;

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("Scene Viewport");

		m_IsInFocus = ImGui::IsWindowFocused();
		CheckAndHandleResize(scene);
		RenderViewport(scene);

		uint32_t color = m_Frame->GetColorBuffer();
		ImGui::Image((void*)color, ImVec2(m_PanelWidth, m_PanelHeight), { 0, 1 }, { 1, 0 });

		ImGui::End();
		ImGui::PopStyleVar();
	}

	void SceneViewportPanel::CheckAndHandleResize(const Ref<Scene>& scene) {
		ImVec2 windowSize = ImGui::GetContentRegionAvail();

		if (windowSize.x < 0 || windowSize.y < 0) {
			return;
		}

		if (m_PanelWidth != (int)windowSize.x || m_PanelHeight != (int)windowSize.y) {
			m_PanelWidth = (int)windowSize.x;
			m_PanelHeight = (int)windowSize.y;

			m_Frame->Resize(m_PanelWidth, m_PanelHeight);
			RenderCommands::UpdateViewPort(m_PanelWidth, m_PanelHeight);
			scene->OnViewportResize(m_PanelWidth, m_PanelHeight);
		}
	}

	void SceneViewportPanel::RenderViewport(const Ref<Scene>& scene) {
		m_Frame->Bind();
		RenderCommands::Clear();
		Renderer2D::ResetStats();
		
		scene->RenderScene();

		m_Frame->Unbind();
		m_RendererStats = Renderer2D::GetStats();
	}

}

