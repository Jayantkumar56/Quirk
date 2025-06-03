

#include "QkEditorpch.h"

#include "Quirk.h"
#include "EditorTitleBar.h"
#include "Editor/EditorTheme.h"
#include "EditorFrame.h"

namespace QuirkEditor {

	void EditorTitleBar::SetImguiProperties() {
		ImGuiStyle& style = ImGui::GetStyle();
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, { style.FramePadding.x, 8.0f });
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	}

	void EditorTitleBar::UnSetImguiProperties() {
		ImGui::PopStyleVar(2);
	}

	void EditorTitleBar::OnImguiUiUpdate() {
        EditorFrame* frame = m_EditorFrame.Get();

        auto scene = frame->GetActiveSceneView();
		ImGui::PushStyleColor(ImGuiCol_Border, frame->GetTheme()->GetColor(ColorName::PopupBorder));

		if (ImGui::BeginMenu("File")) {
			if (ImGui::MenuItem("Exit")) {
                Quirk::WindowCloseEvent event;
                Quirk::Application::Get().OnWindowClose(event);
			}

			if (ImGui::MenuItem("Open Scene")) {
                Quirk::FileFilter filters[] = {
					{L"Scene",		L"*.yaml"},
					{L"All",		L"*.*"}
				};

                Quirk::FileDialogSpecification fileDialogSpec;
				fileDialogSpec.Title		 = L"Open Scene";
				fileDialogSpec.FileNameLabel = L"Scene Name";
				fileDialogSpec.Filters		 = filters;
				fileDialogSpec.NoOfFilters	 = sizeof(filters) / sizeof(Quirk::FileFilter);
				fileDialogSpec.ParentWindow  = GetWindow().Get();

				std::filesystem::path filePath;
				if (Quirk::FileDialog::OpenFile(fileDialogSpec, filePath)) {
					scene->DestroyAllEntities();
                    Quirk::SceneSerializer::Deserialize(scene, filePath);
				}
			}

			if (ImGui::MenuItem("Save Scene") && scene != nullptr) {
                Quirk::FileFilter filters[] = {
					{L"Scene",		L"*.yaml"},
					{L"All",		L"*.*"}
				};

                Quirk::FileDialogSpecification fileDialogSpec;
				fileDialogSpec.Title		 = L"Save Scene";
				fileDialogSpec.FileNameLabel = L"Scene Name";
				fileDialogSpec.Filters		 = filters;
				fileDialogSpec.NoOfFilters	 = sizeof(filters) / sizeof(Quirk::FileFilter);
				fileDialogSpec.ParentWindow  = GetWindow().Get();

				std::filesystem::path filePath;
				if (Quirk::FileDialog::SaveFile(fileDialogSpec, filePath)) {
                    Quirk::SceneSerializer::Serialize(scene, filePath);
				}
			}

			ImGui::EndMenu();
		}

		float buttonSize    = 23.0f;
		float buttonPadding = 9.0f;
		ImVec2 windowSize   = ImGui::GetWindowSize();

		auto framePadding = ImGui::GetStyle().FramePadding;
		float buttonY     = (windowSize.y - buttonSize - (2 * framePadding.y)) * 0.5f;
		float buttonX     = windowSize.x - (3 * buttonSize) - 2 * (buttonPadding + framePadding.x);

		ImGui::SetCursorPos({ buttonX, buttonY });
		ImTextureID minimiseIconId = (ImTextureID)(intptr_t)m_MinimiseIcon->GetRendererId();
		ImGui::ImageButton(minimiseIconId, { buttonSize, buttonSize }, { 0, 1 }, { 1, 0 });
		SetCursorOverMinimiseButton(ImGui::IsItemHovered());

		ImGui::SetCursorPos({ buttonX + buttonSize + buttonPadding, buttonY });
		ImTextureID maximiseIconId = (ImTextureID)(intptr_t)m_MaximiseIcon->GetRendererId();
		ImGui::ImageButton(maximiseIconId, { buttonSize, buttonSize }, { 0, 1 }, { 1, 0 });
		SetCursorOverMaximiseButton(ImGui::IsItemHovered());

		ImGui::SetCursorPos({ buttonX + 2 * (buttonSize + buttonPadding), buttonY });
		ImTextureID closeIconId = (ImTextureID)(intptr_t)m_CloseIcon->GetRendererId();
		ImGui::ImageButton(closeIconId, { buttonSize, buttonSize }, { 0, 1 }, { 1, 0 });
		SetCursorOverCloseButton(ImGui::IsItemHovered());

		ImGui::PopStyleColor();
	}

}
