


#include "LauncherFrame.h"
#include "imgui_internal.h"

#include "Editor/EditorFrame.h"
#include "QuirkEditorApp.h"

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
	static bool TextColorButton(const char* label, ImVec2 buttonSize, ImU32 color, ImU32 hoverColor, ImU32 activeColor, ImU32 borderColor = 0);

	void LauncherFrame::OnImguiUiUpdate() {
		ImGuiWindowClass windowClass;
		windowClass.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoTabBar;
		ImGui::SetNextWindowClass(&windowClass);

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(35.0f, 0.0f));
		ImGui::Begin("Project Selection Panel");

		switch (m_State) {
			case LauncherState::MainMenu:		DrawMainMenu();            break;
			case LauncherState::ProjectForm:    DrawProjectCreationForm(); break;
		}

		// telling window if it can move with cursor 
		// should only set true in requred condition since resetting is done 
		// in every cycle in the OnUpdate() of the FrameManager
		if (ImGui::IsWindowHovered() && !ImGui::IsAnyItemHovered()) {
			GetWindow().SetCanMoveWithCursor(true);
		}

		ImGui::End();
		ImGui::PopStyleVar();
	}

	void LauncherFrame::DrawMainMenu() {
		// main title 
		ImguiUIUtility::Text("Quirk Game Engine", FontManager::GetFont(FontWeight::Medium, 50));

		// padding between main title of the window and rest content
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 30.0f);

		ImVec2 cellPadding = ImGui::GetStyle().CellPadding;
		ImGui::GetStyle().CellPadding = ImVec2(50.0f, 8.0f);  // for Main Table

		if (ImGui::BeginTable("Main Table", 2)) {
			// paramters object used to create button with customised parameters
			ImageTextButtonParameters parameters;
			parameters.labelFont           = FontManager::GetFont(FontWeight::Medium, 29);
			parameters.descriptionFont     = FontManager::GetFont(FontWeight::Regular, 20);
			parameters.imageSize.x         = 35.0f;
			parameters.imageSize.y         = 35.0f;
			parameters.buttonColor         = 0xff2a2822;
			parameters.hoverColor          = 0xff6a5713;
			parameters.imageContentPadding = 15.0f;

			ImGui::TableNextColumn(); // 1st column
			{
				const auto& recentProjects = Project::GetRecentProjectsList();

				ImguiUIUtility::Text("Recent Projects", FontManager::GetFont(FontWeight::Medium, 29));

				// padding betwen title "Recent Projects" and content
				ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 20.0f);

				// All of the contents (Recent Porjects)
				if (recentProjects.empty()) {
					ImGui::PushFont(FontManager::GetFont(FontWeight::Regular, 23));
					ImGui::TextColored({ 0.812f, 0.816f, 0.78f, 1.0f }, "No Recent Projects!");
					ImGui::PopFont();
				}
				else {
					auto availRgn = ImGui::GetContentRegionAvail();

					parameters.buttonSize.x = availRgn.x - ImGui::GetStyle().ScrollbarSize - 5;
					parameters.buttonSize.y = 80.0f;

					ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
					ImGui::BeginChild("ScrollingRegion", ImVec2(availRgn.x, availRgn.y - 50));

					for (const auto& project : recentProjects) {
						std::string projPath = project.Path.string();

						parameters.label = project.Title.c_str();
						parameters.description = projPath.c_str();
						parameters.imgId = (ImTextureID)(intptr_t)m_ProjectIcon->GetRendererId();

						// Recent project button
						if (ImageTextButton(parameters)) {
							std::filesystem::path path = project.Path / (project.Title + ".qkproj");
							Project::Load(path);

							auto app = (QuirkEditorApp*)&Application::Get();
							app->LaunchEditor();

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
			parameters.imageSize.x         = 50.0f;
			parameters.imageSize.y         = 50.0f;
			parameters.buttonSize.x        = 400.0f;
			parameters.buttonSize.y        = 100.0f;
			parameters.buttonColor         = 0xff3b382f;
			parameters.hoverColor          = 0xff276243;
			parameters.imageContentPadding = 15.0f;

			ImGui::TableNextColumn();   // 2nd Column
			{
				ImguiUIUtility::Text("Get Started", FontManager::GetFont(FontWeight::Medium, 29));

				// padding betwen title "Get Started" and content
				ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 20.0f);

				// All of the contents
				{
					parameters.label = "Open a project";
					parameters.description = "Navigate and open an existing project from local disk.";
					parameters.imgId = (ImTextureID)(intptr_t)m_OpenProjectIcon->GetRendererId();

					// Open project button
					if (ImageTextButton(parameters)) {
						FileDialogSpecification fileDialogSpec;
						fileDialogSpec.Title = L"Open Project";
						fileDialogSpec.FileNameLabel = L"Project Folder";
						fileDialogSpec.ParentWindow = &GetWindow();

						std::filesystem::path filePath;
						if (FileDialog::OpenFolder(fileDialogSpec, filePath)) {
							const auto& proj = Project::AddRecentProject(filePath);

							if (proj != "") {
								Project::Load(proj);

								auto app = (QuirkEditorApp*)&Application::Get();
								app->LaunchEditor();

								// AddFrame adds the frame and makes that context to be current
								// so making launcher frame to be the current context before proceeding
								MakeContextCurrent();

								CloseFrame();
							}
						}
					}

					parameters.label = "Create a new project";
					parameters.description = "Select name and settings to get started.";
					parameters.imgId = (ImTextureID)(intptr_t)m_CreateProjectIcon->GetRendererId();

					// Create project button
					if (ImageTextButton(parameters)) {
						// switching to project creation state to draw the form
						m_State = LauncherState::ProjectForm;
					}
				}
			}

			ImGui::EndTable();
		}

		ImGui::GetStyle().CellPadding = cellPadding; // for Main Table
	}

	void LauncherFrame::DrawProjectCreationForm() {
		// main title 
		ImguiUIUtility::Text("Create your new project", FontManager::GetFont(FontWeight::Medium, 50));

		// padding between main title of the window and rest content
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 30.0f);

		// styles for InputText fields below
		ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.184f, 0.22f, 0.231f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_Border,  ImVec4(0.263f, 0.427f, 0.408f, 1.0f));
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding,    ImVec2(15.0f, 7.0f));
		ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize,   1.0f             );

		// Project Name field
		ImguiUIUtility::Text("Project Name", FontManager::GetFont(FontWeight::Regular, 23));
		ImGui::InputText("##ProjectName", (char*)m_TempProject.Title.c_str(), m_TempProject.Title.length());

		// padding between the two fields
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 20.0f);

		// Project Path field
		ImguiUIUtility::Text("Project Path", FontManager::GetFont(FontWeight::Regular, 23));
		ImGui::InputText("##ProjectPath", (char*)m_TempProjPath.c_str(), m_TempProjPath.length());

		ImGui::SameLine();
		// button for opening folder selection dialog
		if (TextColorButton(". . .", { 50.0f, 35.0f }, 0xff575346, 0xff3b382f, 0xff48463a, 0xff0ca403)) {
			FileDialogSpecification fileDialogSpec;
			fileDialogSpec.Title = L"Open Project";
			fileDialogSpec.FileNameLabel = L"Project Folder";
			fileDialogSpec.ParentWindow = &GetWindow();

			std::filesystem::path filePath;
			if (FileDialog::OpenFolder(fileDialogSpec, filePath)) {
				m_TempProjPath = filePath.string();
				m_TempProjPath.resize(512);
			}
		}

		ImGui::PopStyleVar(2);
		ImGui::PopStyleColor(2);

		// putting buttons below to the extreme corner of the window
		ImVec2 buttonSize  = { 120.0f, 50.0f };
		ImVec2 windSize    = ImGui::GetWindowSize();

		ImVec2 buttonPos = {
			windSize.x - buttonSize.x * 2 - 50.0f,
			windSize.y - buttonSize.y - 50.f
		};

		ImGui::SetCursorPos(buttonPos);

		// back button for navigating back
		if (TextColorButton("Back", buttonSize, 0xff575346, 0xff3b382f, 0xff48463a)) {
			m_State = LauncherState::MainMenu;
		}

		// if the directory choosen for project is valid or not
		// if not valid show popup telling wrong directory choosen
		bool isValidDirectory = true;

		ImGui::SameLine();
		// create button to create the new project
		if (TextColorButton("Create", buttonSize, 0xff575346, 0xff3b382f, 0xff48463a)) {
			// removing the extra '\0' characters from the strings
			m_TempProject.Title.erase(m_TempProject.Title.find_last_not_of('\0') + 1);
			m_TempProjPath.erase(m_TempProjPath.find_last_not_of('\0') + 1);

			m_TempProject.Path = m_TempProjPath + "/" + m_TempProject.Title;

			if (std::filesystem::is_directory(m_TempProject.Path.parent_path())) {
				const auto& proj = Project::CreateNewProject(m_TempProject);

				if (proj) {
					auto app = (QuirkEditorApp*)&Application::Get();
					app->LaunchEditor();

					Project::AddRecentProject(m_TempProject.Path);

					// AddFrame adds the frame and makes that context to be current
					// so making launcher frame to be the current context before proceeding
					MakeContextCurrent();

					CloseFrame();
				}
			}
			else {
				isValidDirectory = false;
			}
		}

		if (!isValidDirectory) {
			// TODO: if the directory choosen for project is valid or not show popup telling wrong directory choosen
		}
	}

	static bool TextColorButton(const char* label, ImVec2 buttonSize, ImU32 color, ImU32 hoverColor, ImU32 activeColor, ImU32 borderColor) {
		ImGui::PushID(label);
		ImVec2 labelSize = ImGui::CalcTextSize(label);
		ImVec2 cursorPos = ImGui::GetCursorScreenPos();

		bool isClicked = ImGui::InvisibleButton(label, buttonSize);
		bool isHovered = ImGui::IsItemHovered();
		bool isActive  = isHovered && ImGui::IsMouseDown(0);

		ImDrawList* drawList = ImGui::GetWindowDrawList();
		ImU32 buttonColor    = isHovered ? isActive ? activeColor : hoverColor : color;
		
		// for border
		if (borderColor) {
			drawList->AddRectFilled(
				{ cursorPos.x - 1.0f, cursorPos.y - 1.0f } , 
				{ cursorPos.x + buttonSize.x + 1.0f, cursorPos.y + buttonSize.y + 1.0f },
				borderColor
			);
		}

		drawList->AddRectFilled(cursorPos, { cursorPos.x + buttonSize.x, cursorPos.y + buttonSize.y }, buttonColor);

		ImVec2 textPos = { cursorPos.x + (buttonSize.x - labelSize.x) * 0.5f, cursorPos.y + (buttonSize.y - labelSize.y) * 0.5f };
		drawList->AddText(textPos, 0xFFFFFFFF, label);

		ImGui::PopID();
		return isClicked;
	}

	static bool ImageTextButton(ImageTextButtonParameters& p) {
		ImGui::PushID(p.description);

		ImVec2  buttonPadding = { 20.0f, 10.0f };
		float availableWidthForText = p.buttonSize.x - p.imageSize.x - p.imageContentPadding - 2 * buttonPadding.x;

		ImGui::PushFont(p.labelFont);
		ImVec2 labelSize = ImGui::CalcTextSize(p.label, 0, false, availableWidthForText);
		ImGui::PopFont();

		ImGui::PushFont(p.descriptionFont);
		ImVec2 descriptionSize = ImGui::CalcTextSize(p.description, 0, false, availableWidthForText);
		ImGui::PopFont();

		ImVec2 cursorPos = ImGui::GetCursorScreenPos();

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

