

#include "EditorFrame.h"
#include "FontManager.h"
#include "Theme.h"

namespace Quirk {

	bool EditorFrame::OnEvent(Event& event){
		return false;
	}

	void EditorFrame::OnUpdate(){

	}

	void EditorFrame::OnImguiUiUpdate(){
		// Disabling alt key for imgui to prevent navigation with alt key (problems when using editor cotrols)
		ImGui::SetKeyOwner(ImGuiKey_LeftAlt, ImGuiKeyOwner_Any, ImGuiInputFlags_LockThisFrame);
	}

}

