

#include "QkEditorpch.h"

#include "SceneViewportPanel.h"
#include "Editor/EditorFrame.h"
#include "Base/SelectionContext.h"
#include "Editor/EditorFrameResourceManager.h"

#include "Core/Input/Input.h"

#include <filesystem>

namespace QuirkEditor {

	SceneViewportPanel::SceneViewportPanel(EditorFrame* frame) :
			Panel    ( "Scene Viewport"                                   ),
			m_Camera ( 45.0f, m_PanelSize.x / m_PanelSize.y, 1.0f, 100.0f ),
			m_FrameBuffer  ( 
                frame->GetRenderSystem()->GetFactory()->CreateFrameBuffer({
                    static_cast<uint32_t>(m_PanelSize.x),
                    static_cast<uint32_t>(m_PanelSize.y)
                })
            ),
            m_SceneRenderer ( frame->GetActiveSceneView(), m_FrameBuffer ),
            m_EditorFrame   ( frame                                      )
	{
        Quirk::RenderCommands::UpdateViewPort(
            static_cast<uint32_t>(m_PanelSize.x),
            static_cast<uint32_t>(m_PanelSize.y)
        );

        m_FrameBuffer->SetAttachments({
			{ Quirk::RHI::FrameBufferTextureType::RGBA_8,             { .RGBA = {0.10156f, 0.17968f, 0.20703f, 1.0f} } },
			{ Quirk::RHI::FrameBufferTextureType::RED_INTEGER,        { .RedInteger = -1   }                           },
			{ Quirk::RHI::FrameBufferTextureType::DEPTH_24_STENCIL_8, { .DepthValue = 1.0f }                           }
		});
	}

	bool SceneViewportPanel::OnEvent(Quirk::Event& event) {
        auto editorMode = m_EditorFrame->GetEditorMode();

		if (m_IsInFocus && editorMode == EditorMode::Edit) {
			return m_Camera.OnEvent(event);
		}

		return false;
	}

	void SceneViewportPanel::OnUpdate() {
        auto editorMode = m_EditorFrame->GetEditorMode();

		if (m_IsInFocus && editorMode == EditorMode::Edit)
			m_ControllingCamera = m_Camera.OnUpdate();
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

	void SceneViewportPanel::OnUiUpdate() {
        auto editorFrame = m_EditorFrame;
        auto editorMode  = editorFrame->GetEditorMode();
        auto scene       = editorFrame->GetActiveSceneView();

		m_IsInFocus = ImGui::IsWindowFocused();
		CheckAndHandleResize(scene);

		//MenuBar(scene);

        // Rendering the Scene
        m_SceneRenderer.Render(m_Camera.GetProjectionView(), m_Camera.GetPosition());

		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, { 0.0f, 0.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered,   ImVec4(0.0f, 0.0f, 0.0f, 0.0f)); 
		ImGui::PushStyleColor(ImGuiCol_ButtonActive,    ImVec4(0.0f, 0.0f, 0.0f, 0.0f));  

		ImVec2 imagePos = ImGui::GetCursorPos();
		ImTextureID frameBuffer = (ImTextureID)(intptr_t)m_FrameBuffer->GetColorAttachment(0);
		bool clickedOnImage = ImGui::ImageButton(
			"viewportimage",
			frameBuffer,
			m_PanelSize,
			{ 0, 1 },
			{ 1, 0 }
		);

		ImGui::PopStyleColor(2);
		ImGui::PopStyleVar();

		if (editorMode == EditorMode::Edit) {
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
                        Quirk::Entity entity((entt::entity)entityId, scene.Get());
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
                    SelectionContext::SetSelected(Quirk::Entity((entt::entity)entityId, scene.Get()));
			}
		}
	}

	void SceneViewportPanel::MenuBar(Quirk::View<Quirk::Scene> scene) {
        auto editorFrame     = m_EditorFrame;
        auto editorMode      = editorFrame->GetEditorMode();
        auto resourceManager = editorFrame->GetResourceManager();

		ImGuiWindowClass window_class;
		window_class.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoTabBar;
		ImGui::SetNextWindowClass(&window_class);

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGuiWindowFlags flags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoDecoration;
		ImGui::Begin("Scene Viewport MenuBar", NULL, flags);

		ImTextureID playButtonIconId = resourceManager->GetIconPlay();
		if (editorMode == EditorMode::Play)
			playButtonIconId = resourceManager->GetIconPause();

		float buttonHeight = ImGui::GetContentRegionAvail().y - 4.0f;
		if (ImGui::ImageButton("playButton", playButtonIconId, { buttonHeight, buttonHeight }, { 0, 1 }, { 1, 0 })) {
			// on click transition from one state to other (from edit to play)
		}

		ImGui::End();
		ImGui::PopStyleVar();
	}

	void SceneViewportPanel::CheckAndHandleResize(Quirk::View<Quirk::Scene> scene) {
		ImVec2 windowSize = ImGui::GetContentRegionAvail();

		if (windowSize.x < 0 || windowSize.y < 0) {
			return;
		}

		if ((uint32_t)m_PanelSize.x != (uint32_t)windowSize.x || (uint32_t)m_PanelSize.y != (uint32_t)windowSize.y) {
            uint32_t width  = static_cast<uint32_t>(windowSize.x);
            uint32_t height = static_cast<uint32_t>(windowSize.y);

            m_PanelSize.x = (float)width;
            m_PanelSize.y = (float)height;

			m_Camera.SetViewportSize(m_PanelSize.x, m_PanelSize.y);

            m_FrameBuffer->Resize(width, height);
            Quirk::RenderCommands::UpdateViewPort(width, height);

			scene->OnViewportResize(width, height);
		}
	}

	int SceneViewportPanel::GetEntityIdOnClick(const ImVec2& imagePos) {
        Quirk::View<Quirk::Window> window = m_EditorFrame->GetWindow();
		ImVec2 windowPos = ImGui::GetWindowPos();
		windowPos        = { windowPos.x - window->GetPosX(), windowPos.y - window->GetPosY() };
		ImVec2 mousePos  = { Quirk::Input::MouseCurrentX() - windowPos.x, Quirk::Input::MouseCurrentY() - windowPos.y };

		// mouse position on the image button
		mousePos   = { mousePos.x - imagePos.x, mousePos.y - imagePos.y };
		// inverting the y axis for mouse coords
		// 2 added because of slight visual error		TO DO: find this error
		mousePos.y = m_PanelSize.y - mousePos.y - 2;

		int entityId = 0;
        m_FrameBuffer->GetColorPixelData(1, (int)mousePos.x, (int)mousePos.y, 1, 1, &entityId, sizeof(entityId));
		return entityId;
	}

}

