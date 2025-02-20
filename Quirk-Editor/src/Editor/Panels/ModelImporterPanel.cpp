

#include "ModelImporterPanel.h"


namespace Quirk {

	void ModelImporter::SetImguiProperties() {
		ImGuiWindowClass window_class;
		window_class.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoWindowMenuButton;
		ImGui::SetNextWindowClass(&window_class);
	}

	void ModelImporter::UnSetImguiProperties() {

	}

	void ModelImporter::OnImguiUiUpdate() {

	}

}
