


#include "Quirk.h"
#include "TitleBar.h"
#include "Theme.h"

namespace Quirk {

	void TitleBar::OnImguiUiUpdate(Ref<Scene>& scene) {
		if (ImGui::BeginMainMenuBar()) {
			ImGui::PushStyleColor(ImGuiCol_Border, Theme::GetColor(ColorName::PopupBorder));

			if (ImGui::BeginMenu("File")) {
				if (ImGui::MenuItem("Exit")) {
					WindowCloseEvent event;
					Application::Get().OnWindowClose(event);
				}

				if (ImGui::MenuItem("Open")) {
					FileFilter filters[] = {
						{L"Scene",		L"*.yaml"},
						{L"All",		L"*.*"}
					};

					FileDialogSpecification fileDialogSpec;
					fileDialogSpec.Title			= L"Open Scene";
					fileDialogSpec.FileNameLabel	= L"Scene Name";
					fileDialogSpec.Filters			= filters;
					fileDialogSpec.NoOfFilters		= sizeof(filters) / sizeof(FileFilter);

					std::wstring filePath = FileDialog::OpenFile(fileDialogSpec);
				}

				if (ImGui::MenuItem("Save")) {
					FileFilter filters[] = {
						{L"Scene",		L"*.yaml"},
						{L"All",		L"*.*"}
					};

					FileDialogSpecification fileDialogSpec;
					fileDialogSpec.Title			= L"Save Scene";
					fileDialogSpec.FileNameLabel	= L"Scene Name";
					fileDialogSpec.Filters			= filters;
					fileDialogSpec.NoOfFilters		= sizeof(filters) / sizeof(FileFilter);

					std::wstring filePath = FileDialog::SaveFile(fileDialogSpec);
					SceneSerializer::Serialize(scene, filePath);
				}

				ImGui::EndMenu();
			}

			ImGui::PopStyleColor();
			ImGui::EndMainMenuBar();
		}
	}

}
