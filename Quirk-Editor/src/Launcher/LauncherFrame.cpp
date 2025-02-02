


#include "LauncherFrame.h"

namespace Quirk {

	void LauncherFrame::OnAttach() {

	}

	void LauncherFrame::OnDetach() {

	}

	bool LauncherFrame::OnEvent(Event& event) {

		return false;
	}

	void LauncherFrame::OnUpdate() {

	}

	void LauncherFrame::OnImguiUiUpdate() {
		ImGuiWindowClass windowClass;
		windowClass.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoTabBar;
		ImGui::SetNextWindowClass(&windowClass);

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(25.0f, 0.0f));
		ImGui::Begin("Project Selection Panel");

		ImguiUIUtility::Text("Quirk  Game  Engine", m_FontManager.GetFont(FontWeight::Regular, 50));

		// telling window if it can move with cursor 
		// should only set true in requred condition since resetting is done 
		// in every cycle in the OnUpdate() of the FrameManager
		if (ImGui::IsWindowHovered() && !ImGui::IsAnyItemHovered()) {
			GetWindow().SetCanMoveWithCursor(true);
		}



		ImGui::End();
		ImGui::PopStyleVar();
	}

	void LauncherFrame::SetColorTheme() {
		auto& colors = ImGui::GetStyle().Colors;

		colors[ImGuiCol_WindowBg]                    = ImVec4{ 0.133f, 0.157f, 0.165f, 1.0f };
		colors[ImGuiCol_MenuBarBg]                   = ImVec4{ 0.133f, 0.157f, 0.165f, 1.0f };
											         
		// Title							         
		colors[ImGuiCol_TitleBg]                     = ImVec4{ 0.039f, 0.035f, 0.047f, 1.0f };
		colors[ImGuiCol_TitleBgActive]               = ImVec4{ 0.086f, 0.09f, 0.09f, 1.0f };
		colors[ImGuiCol_TitleBgCollapsed]            = ImVec4{ 0.086f, 0.09f, 0.09f, 1.0f };
											         
		// Border							         
		colors[ImGuiCol_Border]                      = ImVec4{ 0.0f, 0.0f, 0.0f, 1.0f };
		colors[ImGuiCol_BorderShadow]                = ImVec4{ 0.0f, 0.0f, 0.0f, 1.0f };
											         
		// Resize Grip						         
		colors[ImGuiCol_ResizeGrip]                  = ImVec4{ 0.0f, 0.0f, 0.0f, 1.0f };
		colors[ImGuiCol_ResizeGripHovered]           = ImVec4{ 0.0f, 0.0f, 0.0f, 1.0f };
		colors[ImGuiCol_ResizeGripActive]            = ImVec4{ 0.0f, 0.0f, 0.0f, 1.0f };
											         
		// Slider							         
		colors[ImGuiCol_SliderGrab]                  = ImVec4{ 0.44f, 0.37f, 0.61f, 0.54f };
		colors[ImGuiCol_SliderGrabActive]            = ImVec4{ 0.74f, 0.58f, 0.98f, 0.54f };
											         
		// Text								         
		colors[ImGuiCol_Text]                        = ImVec4{ 1.0f, 1.0f, 1.0f, 1.0f };
		colors[ImGuiCol_TextDisabled]                = ImVec4{ 0.5f, 0.5f, 0.5f, 1.0f };
											         
		// Headers							         
		colors[ImGuiCol_Header]                      = ImVec4{ 0.608f, 0.855f, 0.22f, 1.0f };
		colors[ImGuiCol_HeaderHovered]               = ImVec4{ 0.161f, 0.749f, 0.071f, 1.0f };
		colors[ImGuiCol_HeaderActive]                = ImVec4{ 0.086f, 0.757f, 0.447f, 1.0f };
											         
		// Buttons							         
		colors[ImGuiCol_Button]                      = ImVec4{ 0.0f, 0.0f, 0.0f, 0.0f };
		colors[ImGuiCol_ButtonHovered]               = ImVec4{ 0.192f, 0.337f, 0.349f, 1.0f };
		colors[ImGuiCol_ButtonActive]                = ImVec4{ 0.153f, 0.255f, 0.337f, 1.0f };
		colors[ImGuiCol_CheckMark]                   = ImVec4{ 0.74f, 0.58f, 0.98f, 1.0f };
											         
		// Popups							         
		colors[ImGuiCol_PopupBg]                     = ImVec4{ 0.102f, 0.078f, 0.137f, 1.0f };
											         
		// Frame BG							         
		colors[ImGuiCol_FrameBg]                     = ImVec4{ 0.19f, 0.2f, 0.25f, 1.0f };
		colors[ImGuiCol_FrameBgHovered]              = ImVec4{ 0.19f, 0.2f, 0.25f, 1.0f };
		colors[ImGuiCol_FrameBgActive]               = ImVec4{ 0.16f, 0.16f, 0.21f, 1.0f };
											         
		// Tabs								         
		colors[ImGuiCol_Tab]                         = ImVec4{ 0.16f, 0.16f, 0.21f, 1.0f };
		colors[ImGuiCol_TabHovered]                  = ImVec4{ 0.24f, 0.24f, 0.32f, 1.0f };
		colors[ImGuiCol_TabActive]                   = ImVec4{ 0.2f, 0.22f, 0.27f, 1.0f };
		colors[ImGuiCol_TabUnfocused]                = ImVec4{ 0.16f, 0.16f, 0.21f, 1.0f };
		colors[ImGuiCol_TabUnfocusedActive]          = ImVec4{ 0.16f, 0.16f, 0.21f, 1.0f };

		colors[ImGuiCol_TabSelectedOverline]         = ImVec4{ 0.09f, 0.09f, 0.15f, 1.0f };
		colors[ImGuiCol_TabDimmedSelectedOverline]   = ImVec4{ 0.09f, 0.09f, 0.15f, 1.0f };

		// Scrollbar
		colors[ImGuiCol_ScrollbarBg]                 = ImVec4{ 0.1f, 0.1f, 0.13f, 1.0f };
		colors[ImGuiCol_ScrollbarGrab]               = ImVec4{ 0.16f, 0.16f, 0.21f, 1.0f };
		colors[ImGuiCol_ScrollbarGrabHovered]        = ImVec4{ 0.19f, 0.2f, 0.25f, 1.0f };
		colors[ImGuiCol_ScrollbarGrabActive]         = ImVec4{ 0.24f, 0.24f, 0.32f, 1.0f };

		// Seperator
		colors[ImGuiCol_Separator]                   = ImVec4{ 0.44f, 0.37f, 0.61f, 1.0f };
		colors[ImGuiCol_SeparatorHovered]            = ImVec4{ 0.74f, 0.58f, 0.98f, 1.0f };
		colors[ImGuiCol_SeparatorActive]             = ImVec4{ 0.84f, 0.58f, 1.0f, 1.0f };

		// Docking
		colors[ImGuiCol_DockingPreview]              = ImVec4{ 0.44f, 0.37f, 0.61f, 1.0f };
	}

}

