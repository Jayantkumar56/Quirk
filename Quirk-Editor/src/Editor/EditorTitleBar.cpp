


#include "Quirk.h"
#include "EditorTitleBar.h"
#include "Theme.h"
#include "EditorFrame.h"

namespace Quirk {

	void EditorTitleBar::SetImguiProperties() {
		ImGuiStyle& style = ImGui::GetStyle();
		style.FramePadding.y = 8.0f;

		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	}

	void EditorTitleBar::UnSetImguiProperties() {
		ImGuiStyle& style = ImGui::GetStyle();
		style.FramePadding.y = 5.0f;

		ImGui::PopStyleVar();
	}

	void EditorTitleBar::OnImguiUiUpdate() {
		Ref<Scene>& scene = ((EditorFrame*)GetParentFrame())->GetMainScene();
		ImGui::PushStyleColor(ImGuiCol_Border, Theme::GetColor(ColorName::PopupBorder));

		if (ImGui::BeginMenu("File")) {
			if (ImGui::MenuItem("Exit")) {
				WindowCloseEvent event;
				Application::Get().OnWindowClose(event);
			}

			if (ImGui::MenuItem("Open Scene")) {
				FileFilter filters[] = {
					{L"Scene",		L"*.yaml"},
					{L"All",		L"*.*"}
				};

				FileDialogSpecification fileDialogSpec;
				fileDialogSpec.Title		 = L"Open Scene";
				fileDialogSpec.FileNameLabel = L"Scene Name";
				fileDialogSpec.Filters		 = filters;
				fileDialogSpec.NoOfFilters	 = sizeof(filters) / sizeof(FileFilter);
				fileDialogSpec.ParentWindow  = &((EditorFrame*)GetParentFrame())->GetWindow();

				std::filesystem::path filePath;
				if (FileDialog::OpenFile(fileDialogSpec, filePath)) {
					scene->DestroyAllEntities();
					SceneSerializer::Deserialize(scene, filePath);
				}
			}

			if (ImGui::MenuItem("Save Scene") && scene != nullptr) {
				FileFilter filters[] = {
					{L"Scene",		L"*.yaml"},
					{L"All",		L"*.*"}
				};

				FileDialogSpecification fileDialogSpec;
				fileDialogSpec.Title		 = L"Save Scene";
				fileDialogSpec.FileNameLabel = L"Scene Name";
				fileDialogSpec.Filters		 = filters;
				fileDialogSpec.NoOfFilters	 = sizeof(filters) / sizeof(FileFilter);
				fileDialogSpec.ParentWindow = &((EditorFrame*)GetParentFrame())->GetWindow();

				std::filesystem::path filePath;
				if (FileDialog::SaveFile(fileDialogSpec, filePath)) {
					SceneSerializer::Serialize(scene, filePath);
				}
			}

			ImGui::EndMenu();
		}

		float buttonSize    = 25.0f;
		float buttonPadding = 10.0f;
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
