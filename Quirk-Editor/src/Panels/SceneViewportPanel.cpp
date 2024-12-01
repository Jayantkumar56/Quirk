


#include "SceneViewportPanel.h"
#include "FontManager.h"
#include "Core/Input/Input.h"
#include <filesystem>

namespace Quirk {

	SceneViewportPanel::SceneViewportPanel(uint16_t width, uint16_t height) :
			m_PanelWidth	   (width),
			m_PanelHeight	   (height),
			m_Frame			   (FrameBuffer::Create({ m_PanelWidth, m_PanelHeight })),
			m_RendererStats	   ({ 0, 0 }),
			m_IsInFocus		   (false),
			m_ControllingCamera(false),
			m_Camera		   (45.0f, (float)width / (float)height, 1.0f, 100.0f)
	{
		RenderCommands::UpdateViewPort(m_PanelWidth, m_PanelHeight);
		m_Frame->SetAttachments({
			{ FrameBufferTextureType::RGBA_8,			  { .RGBA = {0.10156f, 0.17968f, 0.20703f, 1.0f} } },
			{ FrameBufferTextureType::RED_INTEGER,        { .RedInteger = -1   }						   },
			{ FrameBufferTextureType::DEPTH_24_STENCIL_8, { .DepthValue = 1.0f }						   }
		});
	}

	bool SceneViewportPanel::OnEvent(Event& event) {
		if (!m_IsInFocus) {
			return false;
		}

		m_Camera.OnEvent(event);

		return false;
	}

	void SceneViewportPanel::OnUpdate(const Ref<Scene>& scene) {
		if (!m_IsInFocus) {
			return;
		}

		m_ControllingCamera = m_Camera.OnUpdate();
		scene->OnUpdate();
	}

	void SceneViewportPanel::OnImguiUiUpdate(Ref<Scene>& scene, Entity& selectedEntity) {
		ImGuiWindowClass window_class;
		window_class.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoTabBar;
		ImGui::SetNextWindowClass(&window_class);

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("Scene Viewport");

		m_IsInFocus = ImGui::IsWindowFocused();
		CheckAndHandleResize(scene);
		RenderViewport(scene);

		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, { 0.0f, 0.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.0f, 0.0f, 0.0f, 0.0f)); 
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));  

		ImVec2 imagePos = ImGui::GetCursorPos();
		bool clickedOnImage = ImGui::ImageButton(
			"viewportimage",
			(ImTextureID)(intptr_t)m_Frame->GetColorAttachment(0),
			ImVec2((float)m_PanelWidth, (float)m_PanelHeight),
			{ 0, 1 },
			{ 1, 0 }
		);

		ImGui::PopStyleColor(2);
		ImGui::PopStyleVar();

		if (ImGui::BeginDragDropTarget()) {
			const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("SCENE_PATH");
			if (payload) {
				scene->DestroyAllEntities();
				SceneSerializer::Deserialize(scene, **(std::filesystem::path**)payload->Data);
			}

			ImGui::EndDragDropTarget();
		}

		if (clickedOnImage && !m_ControllingCamera) {
			Window& window = Application::Get().GetWindow();
			ImVec2 windowPos = ImGui::GetWindowPos();
			windowPos		 = { windowPos.x - window.GetPosX(), windowPos.y - window.GetPosY()};
			ImVec2 mousePos  = { Input::MouseCurrentX() - windowPos.x, Input::MouseCurrentY() - windowPos.y };

			// mouse position on the image button
			mousePos = { mousePos.x - imagePos.x, mousePos.y - imagePos.y };
			// inverting the y axis for mouse coords
			// 2 added because of slight visual error		TO DO: find this error
			mousePos.y = m_PanelHeight - mousePos.y - 2;

			int entityId = 0;
			m_Frame->GetColorPixelData(1, (int)mousePos.x, (int)mousePos.y, &entityId, 1);
			selectedEntity = (entityId == -1) ? Entity() : Entity((entt::entity)entityId, scene.get());
		}

		ImGui::End();
		ImGui::PopStyleVar();
	}

	void SceneViewportPanel::CheckAndHandleResize(const Ref<Scene>& scene) {
		ImVec2 windowSize = ImGui::GetContentRegionAvail();

		if (windowSize.x < 0 || windowSize.y < 0) {
			return;
		}

		if (m_PanelWidth != (int)windowSize.x || m_PanelHeight != (int)windowSize.y) {
			m_PanelWidth  = (int)windowSize.x;
			m_PanelHeight = (int)windowSize.y;

			m_Camera.SetViewportSize(m_PanelWidth, m_PanelHeight);

			m_Frame->Resize(m_PanelWidth, m_PanelHeight);
			RenderCommands::UpdateViewPort(m_PanelWidth, m_PanelHeight);

			scene->OnViewportResize(m_PanelWidth, m_PanelHeight);
		}
	}

	void SceneViewportPanel::RenderViewport(const Ref<Scene>& scene) {
		m_Frame->Bind();
		m_Frame->ClearAttachments();

		Renderer2D::ResetStats();
		scene->RenderScene(m_Camera.GetProjectionView());

		m_Frame->Unbind();
		m_RendererStats = Renderer2D::GetStats();
	}

}

