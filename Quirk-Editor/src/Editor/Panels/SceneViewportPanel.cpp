


#include "SceneViewportPanel.h"
#include "FontManager.h"
#include "Core/Input/Input.h"
#include "Editor/EditorFrame.h"

#include <filesystem>

namespace Quirk {

	SceneViewportPanel::SceneViewportPanel(uint16_t width, uint16_t height) :
			m_RuntimeScene	   (nullptr),
			m_PlayButtonIcon   (Texture2D::Create("assets/Images/play.png")),
			m_PauseButtonIcon  (Texture2D::Create("assets/Images/pause.png")),
			m_PanelWidth	   (width),
			m_PanelHeight	   (height),
			m_Frame			   (FrameBuffer::Create({ m_PanelWidth, m_PanelHeight })),
			m_RendererStats	   ({ 0, 0 }),
			m_IsInFocus		   (false),
			m_ControllingCamera(false),
			m_Camera		   (45.0f, (float)width / (float)height, 1.0f, 100.0f),
		    m_SceneState	   (SceneState::Edit)
	{
		RenderCommands::UpdateViewPort(m_PanelWidth, m_PanelHeight);
		m_Frame->SetAttachments({
			{ FrameBufferTextureType::RGBA_8,			  { .RGBA = {0.10156f, 0.17968f, 0.20703f, 1.0f} } },
			{ FrameBufferTextureType::RED_INTEGER,        { .RedInteger = -1   }						   },
			{ FrameBufferTextureType::DEPTH_24_STENCIL_8, { .DepthValue = 1.0f }						   }
		});
	}

	bool SceneViewportPanel::OnEvent(Event& event) {
		if (m_IsInFocus && m_SceneState == SceneState::Edit) {
			return m_Camera.OnEvent(event);
		}

		return false;
	}

	void SceneViewportPanel::OnUpdate() {
		Ref<Scene>& scene = ((EditorFrame*)GetParentFrame())->GetMainScene();

		if (m_IsInFocus && m_SceneState == SceneState::Edit)
			m_ControllingCamera = m_Camera.OnUpdate();

		if(m_SceneState == SceneState::Play)
			m_RuntimeScene->OnUpdate();
	}

	void SceneViewportPanel::OnImguiUiUpdate() {
		Ref<Scene>& scene      = ((EditorFrame*)GetParentFrame())->GetMainScene();
		Entity& selectedEntity = ((EditorFrame*)GetParentFrame())->GetSelectedEntity();

		MenuBar(scene);

		ImGuiWindowClass windowClass;
		windowClass.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoTabBar;
		ImGui::SetNextWindowClass(&windowClass);

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("Scene Viewport");

		m_IsInFocus = ImGui::IsWindowFocused();
		CheckAndHandleResize(scene);
		RenderViewport(scene);

		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, { 0.0f, 0.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered,   ImVec4(0.0f, 0.0f, 0.0f, 0.0f)); 
		ImGui::PushStyleColor(ImGuiCol_ButtonActive,    ImVec4(0.0f, 0.0f, 0.0f, 0.0f));  

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

		if (m_SceneState == SceneState::Edit) {
			if (ImGui::BeginDragDropTarget()) {
				const ImGuiPayload* scenePayload = ImGui::AcceptDragDropPayload("SCENE_PATH");
				if (scenePayload) {
					selectedEntity = Entity();
					scene->DestroyAllEntities();
					SceneSerializer::Deserialize(scene, **(std::filesystem::path**)scenePayload->Data);
				}

				const ImGuiPayload* imagePayload = ImGui::AcceptDragDropPayload("IMAGE_PATH");
				if (imagePayload) {
					int entityId = GetEntityIdOnClick(imagePos);

					if (entityId != -1) {
						Entity entity((entt::entity)entityId, scene.get());
						entity.GetComponent<SpriteRendererComponent>().Texture = Texture2D::Create(**(std::filesystem::path**)imagePayload->Data);
					}
				}

				ImGui::EndDragDropTarget();
			}

			if (clickedOnImage && !m_ControllingCamera) {
				int entityId   = GetEntityIdOnClick(imagePos);
				selectedEntity = (entityId == -1) ? Entity() : Entity((entt::entity)entityId, scene.get());
			}
		}

		ImGui::End();
		ImGui::PopStyleVar();
	}

	void SceneViewportPanel::MenuBar(const Ref<Scene>& scene) {
		ImGuiWindowClass window_class;
		window_class.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoTabBar;
		ImGui::SetNextWindowClass(&window_class);

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGuiWindowFlags flags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoDecoration;
		ImGui::Begin("Scene Viewport MenuBar", NULL, flags);

		ImTextureID playButtonIconId = (ImTextureID)(intptr_t)m_PlayButtonIcon->GetRendererId();
		if (m_SceneState == SceneState::Play)
			playButtonIconId = (ImTextureID)(intptr_t)m_PauseButtonIcon->GetRendererId();

		float buttonHeight = ImGui::GetContentRegionAvail().y - 4.0f;
		if (ImGui::ImageButton("playButton", playButtonIconId, { buttonHeight, buttonHeight }, { 0, 1 }, { 1, 0 })) {
			// on click transition from one state to other (from edit to play)
			switch (m_SceneState) {
				case Quirk::SceneState::Edit: OnScenePlay(scene); break;
				case Quirk::SceneState::Play: OnSceneEdit(scene); break;
			}
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
			if (m_RuntimeScene != nullptr) m_RuntimeScene->OnViewportResize(m_PanelWidth, m_PanelHeight);
		}
	}

	void SceneViewportPanel::RenderViewport(const Ref<Scene>& scene) {
		m_Frame->Bind();
		m_Frame->ClearAttachments();

		Renderer2D::ResetStats();

		switch (m_SceneState) {
			case Quirk::SceneState::Edit: scene->RenderSceneEditor(m_Camera.GetProjectionView()); break;
			case Quirk::SceneState::Play: m_RuntimeScene->RenderSceneRuntime(); break;
		}

		m_Frame->Unbind();
		m_RendererStats = Renderer2D::GetStats();
	}

	int SceneViewportPanel::GetEntityIdOnClick(const ImVec2& imagePos) {
		Window& window   = ((EditorFrame*)GetParentFrame())->GetWindow();
		ImVec2 windowPos = ImGui::GetWindowPos();
		windowPos        = { windowPos.x - window.GetPosX(), windowPos.y - window.GetPosY() };
		ImVec2 mousePos  = { Input::MouseCurrentX() - windowPos.x, Input::MouseCurrentY() - windowPos.y };

		// mouse position on the image button
		mousePos   = { mousePos.x - imagePos.x, mousePos.y - imagePos.y };
		// inverting the y axis for mouse coords
		// 2 added because of slight visual error		TO DO: find this error
		mousePos.y = m_PanelHeight - mousePos.y - 2;

		int entityId = 0;
		m_Frame->GetColorPixelData(1, (int)mousePos.x, (int)mousePos.y, &entityId, 1);
		return entityId;
	}

	void SceneViewportPanel::OnSceneEdit(const Ref<Scene>& scene) {
		m_SceneState = SceneState::Edit;

		m_RuntimeScene->OnRuntimeStop();
		m_RuntimeScene = nullptr;
	}

	void SceneViewportPanel::OnScenePlay(const Ref<Scene>& scene) {
		m_SceneState = SceneState::Play;

		m_RuntimeScene = Scene::Copy(scene);
		m_RuntimeScene->OnRuntimeStart();
	}

}

