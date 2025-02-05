


#include "LauncherFrame.h"
#include "imgui_internal.h"

#include "Editor/EditorFrame.h"

#include <iostream>

namespace Quirk {

	struct ImageTextButtonParameters {
		ImTextureID imgId;                         ImVec2      imageSize;
		const char* label;                         const char* description;
		ImFont* labelFont;						   ImFont*     descriptionFont;
		ImU32   buttonColor   = 0x00000000;        ImU32       hoverColor          = 0xFFFFFFFF;
	    float   imageContentPadding = 10.0f; 
		ImVec2  buttonSize = { 0.0f, 0.0f };
	};

	static bool ImageTextButton(ImageTextButtonParameters& p);

	void LauncherFrame::OnImguiUiUpdate() {
		ImGuiWindowClass windowClass;
		windowClass.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoTabBar;
		ImGui::SetNextWindowClass(&windowClass);

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(35.0f, 0.0f));
		ImGui::Begin("Project Selection Panel");

		// main title 
		ImguiUIUtility::Text("Quirk Game Engine", m_FontManager.GetFont(FontWeight::Medium, 50));

		// padding between main title of the window and rest content
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 30.0f);

		ImVec2 cellPadding = ImGui::GetStyle().CellPadding;
		ImGui::GetStyle().CellPadding = ImVec2(50.0f, 8.0f);  // for Main Table

		if (ImGui::BeginTable("Main Table", 2)) {
			// paramters object used to create button with customised parameters
			ImageTextButtonParameters parameters;
			parameters.labelFont                  = m_FontManager.GetFont(FontWeight::Medium, 29);
			parameters.descriptionFont            = m_FontManager.GetFont(FontWeight::Regular, 20);
			parameters.imageSize.x                = 35.0f;
			parameters.imageSize.y                = 35.0f;
			parameters.buttonColor                = 0xff2a2822;
			parameters.hoverColor                 = 0xff6a5713;
			parameters.imageContentPadding        = 15.0f;

			ImGui::TableNextColumn(); // 1st column
			{
				const auto& recentProjects = Project::GetRecentProjectsList();

				ImguiUIUtility::Text("Recent Projects", m_FontManager.GetFont(FontWeight::Medium, 29));

				// padding betwen title "Recent Projects" and content
				ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 20.0f);

				// All of the contents (Recent Porjects)
				if (recentProjects.empty()) {
					ImGui::PushFont(m_FontManager.GetFont(FontWeight::Regular, 23));
					ImGui::TextColored({ 0.812f, 0.816f, 0.78f, 1.0f }, "No Recent Projects!");
					ImGui::PopFont();
				}
				else {
					auto availRgn   = ImGui::GetContentRegionAvail();

					parameters.buttonSize.x = availRgn.x - ImGui::GetStyle().ScrollbarSize - 5;
					parameters.buttonSize.y = 80.0f;

					ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
					ImGui::BeginChild("ScrollingRegion", ImVec2(availRgn.x, availRgn.y -50));

					for (const auto& project : recentProjects) {
						parameters.label       = project.Title.c_str();
						parameters.description = project.Path.c_str();
						parameters.imgId       = (ImTextureID)(intptr_t)m_ProjectIcon->GetRendererId();

						// Recent project button
						if (ImageTextButton(parameters)) {
							std::filesystem::path path = project.Path + "/" + project.Title + ".qkproj";
							Project::Load(path);

							WindowSpecification tempSpec{ 
								.Title             {"Quirk Editor"}, 
								.Width             {1600},				   .Height    {900}, 
								.PosX              {200},				   .PosY      {50}, 
								.VSyncOn           {true},				   .Maximized {false}, 
								.CustomTitleBar    {true},
								.WindowBorderSizeX {4},				       .WindowBorderSizeY {4}, 
								.WindowBorderColor {12, 12, 12},
								.MinWidth          {1600},				   .MinHeight {900}
							};

							Application::Get().AddFrame<EditorFrame>(tempSpec);
							// AddFrame adds the frame and makes that context to be current
							// so making launcher frame to be the current context before proceeding
							MakeContextCurrent();

							CloseFrame();
						}

						// TODO: create a way to remove item from recent project list
					}

					// telling window if it can move with cursor 
					// should only set true in requred condition since resetting is done 
					// in every cycle in the OnUpdate() of the FrameManager
					if (ImGui::IsWindowHovered() && !ImGui::IsAnyItemHovered()) {
						GetWindow().SetCanMoveWithCursor(true);
					}

					ImGui::EndChild();
					ImGui::PopStyleVar();
				}
			}

			// common properties for the custom buttons in the 2nd Column
			parameters.imageSize.x                = 50.0f;
			parameters.imageSize.y                = 50.0f;
			parameters.buttonSize.x               = 400.0f;
			parameters.buttonSize.y               = 100.0f;
			parameters.buttonColor                = 0xff3b382f;
			parameters.hoverColor                 = 0xff276243;
			parameters.imageContentPadding		  = 15.0f;

			ImGui::TableNextColumn();   // 2nd Column
			{
				ImguiUIUtility::Text("Get Started", m_FontManager.GetFont(FontWeight::Medium, 29));

				// padding betwen title "Get Started" and content
				ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 20.0f);

				// All of the contents
				{
					parameters.label       = "Open a project";
					parameters.description = "Navigate and open an existing project from local disk.";
					parameters.imgId       = (ImTextureID)(intptr_t)m_OpenProjectIcon->GetRendererId();

					// Open project button
					if (ImageTextButton(parameters)) {
						// TODO: open the selected project file in the editor
					}

					parameters.label       = "Create a new project";
					parameters.description = "Select name and settings to get started.";
					parameters.imgId       = (ImTextureID)(intptr_t)m_CreateProjectIcon->GetRendererId();

					// Create project button
					if (ImageTextButton(parameters)) {
						// TODO: create a new project and open it in the editor
					}
				}
			}

			ImGui::EndTable();
		}

		ImGui::GetStyle().CellPadding = cellPadding; // for Main Table

		// telling window if it can move with cursor 
		// should only set true in requred condition since resetting is done 
		// in every cycle in the OnUpdate() of the FrameManager
		if (ImGui::IsWindowHovered() && !ImGui::IsAnyItemHovered()) {
			GetWindow().SetCanMoveWithCursor(true);
		}

		ImGui::End();
		ImGui::PopStyleVar();
	}

	static bool ImageTextButton(ImageTextButtonParameters& p) {
		ImVec2  buttonPadding = { 20.0f, 10.0f };
		float availableWidthForText = p.buttonSize.x - p.imageSize.x - p.imageContentPadding - 2 * buttonPadding.x;

		ImGui::PushFont(p.labelFont);
		ImVec2 labelSize = ImGui::CalcTextSize(p.label, 0, false, availableWidthForText);
		ImGui::PopFont();

		ImGui::PushFont(p.descriptionFont);
		ImVec2 descriptionSize = ImGui::CalcTextSize(p.description, 0, false, availableWidthForText);
		ImGui::PopFont();

		ImVec2 cursorPos = ImGui::GetCursorScreenPos();

		ImGui::PushID(p.label);
		bool isClicked = ImGui::InvisibleButton(p.label, p.buttonSize);
		bool isHovered = ImGui::IsItemHovered();

		ImDrawList* drawList = ImGui::GetWindowDrawList();

		ImU32 buttonColor = isHovered ? p.hoverColor : p.buttonColor;
		drawList->AddRectFilled(cursorPos, { cursorPos.x + p.buttonSize.x, cursorPos.y + p.buttonSize.y }, buttonColor);

		drawList->AddImage(
			p.imgId, { cursorPos.x + buttonPadding.x, cursorPos.y + buttonPadding.y },
			{ cursorPos.x + buttonPadding.x + p.imageSize.x, cursorPos.y + buttonPadding.y + p.imageSize.y },
			{ 0, 1 }, { 1, 0 }
		);

		ImVec2 textPos = {
			cursorPos.x + buttonPadding.x + p.imageSize.x + p.imageContentPadding,
			cursorPos.y + buttonPadding.y
		};

		drawList->AddText(p.labelFont, p.labelFont->FontSize, textPos, 0xFFFFFFFF, p.label, NULL, availableWidthForText);

		textPos.x = cursorPos.x + buttonPadding.x + p.imageSize.x + p.imageContentPadding;
		textPos.y = cursorPos.y + labelSize.y + buttonPadding.y;

		drawList->AddText(p.descriptionFont, p.descriptionFont->FontSize, textPos, 0xffc7d0cf, p.description, NULL, availableWidthForText);

		ImGui::PopID();
		return isClicked;
	}

	void LauncherFrame::LoadFonts() {
		auto& io = ImGui::GetIO();

		m_FontManager.LoadFonts();
		m_FontManager.LoadFont(io, FontWeight::Medium, 50);     // for Main Title (Quirk Game Engine)
		m_FontManager.LoadFont(io, FontWeight::Medium, 29);		// for section title ()
		m_FontManager.LoadFont(io, FontWeight::Medium, 25);
		m_FontManager.LoadFont(io, FontWeight::Regular, 20);
		m_FontManager.LoadFont(io, FontWeight::Regular, 23);
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
		colors[ImGuiCol_ScrollbarBg]                 = ImVec4{ 0.275f, 0.325f, 0.341f, 1.0f };
		colors[ImGuiCol_ScrollbarGrab]               = ImVec4{ 0.416f, 0.494f, 0.522f, 1.0f };
		colors[ImGuiCol_ScrollbarGrabHovered]        = ImVec4{ 0.416f, 0.494f, 0.522f, 1.0f };
		colors[ImGuiCol_ScrollbarGrabActive]         = ImVec4{ 0.416f, 0.494f, 0.522f, 1.0f };

		// Seperator
		colors[ImGuiCol_Separator]                   = ImVec4{ 0.44f, 0.37f, 0.61f, 1.0f };
		colors[ImGuiCol_SeparatorHovered]            = ImVec4{ 0.74f, 0.58f, 0.98f, 1.0f };
		colors[ImGuiCol_SeparatorActive]             = ImVec4{ 0.84f, 0.58f, 1.0f, 1.0f };

		// Docking
		colors[ImGuiCol_DockingPreview]              = ImVec4{ 0.44f, 0.37f, 0.61f, 1.0f };
	}

}

