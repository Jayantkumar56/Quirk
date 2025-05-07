

#include "QkEditorpch.h"

#include "SceneViewportPanel.h"
#include "Editor/EditorFrame.h"
#include "Base/SelectionContext.h"

#include "Core/Input/Input.h"

#include <filesystem>

namespace QuirkEditor {

	SceneViewportPanel::SceneViewportPanel(uint16_t width, uint16_t height) :
			Panel              ("Scene Viewport"),
			m_RuntimeScene	   (nullptr),
			m_PlayButtonIcon   (Quirk::TextureImporter::CreateFromImage("assets/Images/play.png")),
			m_PauseButtonIcon  (Quirk::TextureImporter::CreateFromImage("assets/Images/pause.png")),
			m_PanelWidth	   (width),
			m_PanelHeight	   (height),
			m_Frame			   (Quirk::FrameBuffer::Create({ m_PanelWidth, m_PanelHeight })),
			m_RendererStats	   ({ 0, 0 }),
			m_IsInFocus		   (false),
			m_ControllingCamera(false),
			m_Camera		   (45.0f, (float)width / (float)height, 1.0f, 100.0f),
		    m_SceneState	   (SceneState::Edit)
	{
        Quirk::RenderCommands::UpdateViewPort(m_PanelWidth, m_PanelHeight);
		m_Frame->SetAttachments({
			{ Quirk::FrameBufferTextureType::RGBA_8,			 { .RGBA = {0.10156f, 0.17968f, 0.20703f, 1.0f} } },
			{ Quirk::FrameBufferTextureType::RED_INTEGER,        { .RedInteger = -1   }						   },
			{ Quirk::FrameBufferTextureType::DEPTH_24_STENCIL_8, { .DepthValue = 1.0f }						   }
		});
	}

	bool SceneViewportPanel::OnEvent(Quirk::Event& event) {
		if (m_IsInFocus && m_SceneState == SceneState::Edit) {
			return m_Camera.OnEvent(event);
		}

		return false;
	}

	void SceneViewportPanel::SetUiProperties() {
		ImGuiWindowClass window_class;
		window_class.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoWindowMenuButton;
		ImGui::SetNextWindowClass(&window_class);

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	}

	void SceneViewportPanel::UnsetUiProperties() {
		ImGui::PopStyleVar();
	}

	void SceneViewportPanel::OnUpdate() {
        Quirk::Ref<Quirk::Scene>& scene = GetParentFrameAs<EditorFrame>()->GetMainScene();

		if (m_IsInFocus && m_SceneState == SceneState::Edit)
			m_ControllingCamera = m_Camera.OnUpdate();

		if(m_SceneState == SceneState::Play)
			m_RuntimeScene->OnUpdate();
	}

	void SceneViewportPanel::OnUiUpdate() {
        Quirk::Ref<Quirk::Scene>& scene = GetParentFrameAs<EditorFrame>()->GetMainScene();

		//MenuBar(scene);

		m_IsInFocus = ImGui::IsWindowFocused();
		CheckAndHandleResize(scene);
		RenderViewport(scene);

		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, { 0.0f, 0.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered,   ImVec4(0.0f, 0.0f, 0.0f, 0.0f)); 
		ImGui::PushStyleColor(ImGuiCol_ButtonActive,    ImVec4(0.0f, 0.0f, 0.0f, 0.0f));  

		ImVec2 imagePos = ImGui::GetCursorPos();
		ImTextureID frameBuffer = (ImTextureID)(intptr_t)m_Frame->GetColorAttachment(0);
		bool clickedOnImage = ImGui::ImageButton(
			"viewportimage",
			frameBuffer,
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
                    SelectionContext::SetSelected(Quirk::Entity());
					scene->DestroyAllEntities();
                    Quirk::SceneSerializer::Deserialize(scene, **(std::filesystem::path**)scenePayload->Data);
				}

				const ImGuiPayload* imagePayload = ImGui::AcceptDragDropPayload("IMAGE_PATH");
				if (imagePayload) {
					int entityId = GetEntityIdOnClick(imagePos);

					if (entityId != -1) {
                        Quirk::Entity entity((entt::entity)entityId, scene.get());
						entity.GetComponent<Quirk::SpriteRendererComponent>().Texture = Quirk::TextureImporter::CreateFromImage(**(std::filesystem::path**)imagePayload->Data);
					}
				}

				ImGui::EndDragDropTarget();
			}

			if (clickedOnImage && !m_ControllingCamera) {
				int entityId   = GetEntityIdOnClick(imagePos);

                if (entityId == -1)
                    SelectionContext::SetSelected(Quirk::Entity());
                else
                    SelectionContext::SetSelected(Quirk::Entity((entt::entity)entityId, scene.get()));
			}
		}
	}

	void SceneViewportPanel::MenuBar(const Quirk::Ref<Quirk::Scene>& scene) {
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
				case SceneState::Edit: OnScenePlay(scene); break;
				case SceneState::Play: OnSceneEdit(scene); break;
			}
		}

		ImGui::End();
		ImGui::PopStyleVar();
	}

	void SceneViewportPanel::CheckAndHandleResize(const Quirk::Ref<Quirk::Scene>& scene) {
		ImVec2 windowSize = ImGui::GetContentRegionAvail();

		if (windowSize.x < 0 || windowSize.y < 0) {
			return;
		}

		if (m_PanelWidth != (int)windowSize.x || m_PanelHeight != (int)windowSize.y) {
			m_PanelWidth  = (int)windowSize.x;
			m_PanelHeight = (int)windowSize.y;

			m_Camera.SetViewportSize(m_PanelWidth, m_PanelHeight);

			m_Frame->Resize(m_PanelWidth, m_PanelHeight);
            Quirk::RenderCommands::UpdateViewPort(m_PanelWidth, m_PanelHeight);

			scene->OnViewportResize(m_PanelWidth, m_PanelHeight);
			if (m_RuntimeScene != nullptr) m_RuntimeScene->OnViewportResize(m_PanelWidth, m_PanelHeight);
		}
	}

	void SceneViewportPanel::RenderViewport(const Quirk::Ref<Quirk::Scene>& scene) {
		m_Frame->Bind();
		m_Frame->ClearAttachments();

        Quirk::Renderer2D::ResetStats();

		switch (m_SceneState) {
			case SceneState::Edit: scene->RenderSceneEditor(m_Camera.GetProjectionView(), m_Camera.GetPosition()); break;
			case SceneState::Play: m_RuntimeScene->RenderSceneRuntime(); break;
		}

		m_Frame->Unbind();
		m_RendererStats = Quirk::Renderer2D::GetStats();
	}

	int SceneViewportPanel::GetEntityIdOnClick(const ImVec2& imagePos) {
        Quirk::Window& window   = GetWindow();
		ImVec2 windowPos = ImGui::GetWindowPos();
		windowPos        = { windowPos.x - window.GetPosX(), windowPos.y - window.GetPosY() };
		ImVec2 mousePos  = { Quirk::Input::MouseCurrentX() - windowPos.x, Quirk::Input::MouseCurrentY() - windowPos.y };

		// mouse position on the image button
		mousePos   = { mousePos.x - imagePos.x, mousePos.y - imagePos.y };
		// inverting the y axis for mouse coords
		// 2 added because of slight visual error		TO DO: find this error
		mousePos.y = m_PanelHeight - mousePos.y - 2;

		int entityId = 0;
		m_Frame->GetColorPixelData(1, (int)mousePos.x, (int)mousePos.y, 1, 1, &entityId, sizeof(entityId));
		return entityId;
	}

	void SceneViewportPanel::OnSceneEdit(const Quirk::Ref<Quirk::Scene>& scene) {
		m_SceneState = SceneState::Edit;

		m_RuntimeScene->OnRuntimeStop();
		m_RuntimeScene = nullptr;
	}

	void SceneViewportPanel::OnScenePlay(const Quirk::Ref<Quirk::Scene>& scene) {
		m_SceneState = SceneState::Play;

		m_RuntimeScene = Quirk::Scene::Copy(scene);
		m_RuntimeScene->OnRuntimeStart();
	}

}

