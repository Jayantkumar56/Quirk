


#include "Quirk.h"
#include "EditorTitleBar.h"
#include "Theme.h"
#include "EditorFrame.h"

namespace Quirk {

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

		ImGui::PopStyleColor();
	}

}
