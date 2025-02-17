

#include "ContentBrowserPanel.h"
#include "Core/Imgui/ImguiUI.h"
#include "Core/Imgui/ImguiUIUtility.h"

namespace Quirk {

	static bool IsImagePath(const std::string& extension) {
		bool isImage = false;
		isImage = isImage || extension == ".png" || extension == ".Png";
		isImage = isImage || extension == ".JPG" || extension == ".jpg" || extension == ".JPEG" || extension == ".jpeg";
		return isImage;
	}

	void ContentBrowserPanel::SetImguiProperties() {
		ImGuiWindowClass window_class;
		window_class.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoWindowMenuButton;
		ImGui::SetNextWindowClass(&window_class);
	}

	void ContentBrowserPanel::OnImguiUiUpdate() {
		bool updatedCurrentDirectory = false;

		// menubar contains all the navigation buttons and current directory
		DrawMenuBar();

		ImVec2 windowSize = ImGui::GetWindowSize();
		int noOfColumns = (int)((windowSize.x) / 112);

		ImVec2 cellPadding = ImGui::GetStyle().CellPadding;
		ImGui::GetStyle().CellPadding = ImVec2(10.0f, 8.0f);

		if (ImGui::BeginTable("Propertycheckbox", noOfColumns)) {
			for (size_t i = 0; i < m_CurrentDirectoryContent.size(); ++i) {
				ImGui::TableNextColumn();
				std::string directoryName = m_CurrentDirectoryContent[i].filename().string();

				if (std::filesystem::is_directory(m_CurrentDirectoryContent[i])) {
					ImTextureID folderIconId = (ImTextureID)(intptr_t)m_FolderIcon->GetRendererId();
					if (ImGui::ImageButton(directoryName.c_str(), folderIconId, { 100.0f, 100.0f }, { 0, 1 }, { 1, 0 })) {
						m_BackwardNavigationHistory.emplace(m_CurrentDirectory);

						while (!m_ForwardNavigationHistory.empty())
							m_ForwardNavigationHistory.pop();

						m_CurrentDirectory	= m_CurrentDirectoryContent[i];
						updatedCurrentDirectory = true;
					}
				}
				else {
					ImTextureID fileIconId = (ImTextureID)(intptr_t)m_FileIcon->GetRendererId();
					ImGui::ImageButton(directoryName.c_str(), fileIconId, { 100.0f, 100.0f }, { 0, 1 }, { 1, 0 });

					if (ImGui::BeginDragDropSource()) {
						const std::filesystem::path* dragDropFilePathPtr = &m_CurrentDirectoryContent[i];
						uint32_t dummydata = 3;

						std::string extension = m_CurrentDirectoryContent[i].extension().string();

						if(extension == ".yaml")
							ImGui::SetDragDropPayload("SCENE_PATH", &dragDropFilePathPtr, sizeof(dragDropFilePathPtr), ImGuiCond_Once);
						if(IsImagePath(extension))
							ImGui::SetDragDropPayload("IMAGE_PATH", &dragDropFilePathPtr, sizeof(dragDropFilePathPtr), ImGuiCond_Once);

						ImGui::BeginTooltip();
						ImGui::Image(fileIconId, { 100.0f, 100.0f }, { 0, 1 }, { 1, 0 });
						ImGui::EndTooltip();

						ImGui::EndDragDropSource();
					}
				}

				ImguiUIUtility::TextWrappedCentered(directoryName.c_str());
			}

			ImGui::EndTable();
		}
		ImGui::GetStyle().CellPadding = cellPadding;

		if (updatedCurrentDirectory) FetchCurrentDirectoryContent();
	}

	void ContentBrowserPanel::DrawMenuBar() {
		ImVec2 menuBarSize    = { ImGui::GetWindowWidth(), 30.0f };
		ImVec2 menuBarPadding = { 20.0f, 5.0f };

		ImVec2 cursorPos = ImGui::GetWindowPos();
		cursorPos.y += ImGui::GetFrameHeight() + 2.0f;

		ImVec2 menuBarStart = cursorPos;
		ImVec2 menuBarEnd = { cursorPos.x + menuBarSize.x, cursorPos.y + menuBarSize.y };

		ImDrawList* drawList = ImGui::GetWindowDrawList();

		drawList->PushClipRect(menuBarStart, menuBarEnd, true);
		drawList->AddRectFilled(menuBarStart, menuBarEnd, 0xff0b0d0d);

		cursorPos.x += menuBarPadding.x * 0.5f;
		cursorPos.y += menuBarPadding.y * 0.5f;

		ImVec2 buttonSize  = { 25.0f, 25.0f };
		ImVec2 buttonStart = cursorPos;
		ImVec2 buttonEnd   = { cursorPos.x + buttonSize.x, cursorPos.y + buttonSize.y };

		ImU32 buttonHoverColor  = 0xff312d29;
		ImU32 buttonActiveColor = 0xff2d2925;

		// button for Backward Navigation
		{
			ImGui::SetCursorScreenPos(cursorPos);
			if (ImGui::InvisibleButton("backwardButton", buttonSize)) {
				BackwardNavigationCallback();
			}

			// setting texture to the backward button for hovering and normal state
			if (ImGui::IsMouseHoveringRect(buttonStart, buttonEnd)) {
				ImU32 buttonColor = ImGui::IsMouseDown(0) ? buttonActiveColor : buttonHoverColor;
				drawList->AddRectFilled(buttonStart, buttonEnd, buttonColor, 10.0f);
			}

			ImTextureID backwardIconId = (ImTextureID)(intptr_t)m_BackwardIcon->GetRendererId();
			drawList->AddImage(backwardIconId, buttonStart, buttonEnd, { 0, 1 }, { 1, 0 });
		}
		cursorPos.x += buttonSize.x + 3.0f;

		// button for Forward Navigation
		{
			ImGui::SetCursorScreenPos(cursorPos);
			if (ImGui::InvisibleButton("forwardButton", buttonSize)) {
				ForwardNavigationCallback();
			}

			buttonStart = cursorPos;
			buttonEnd.x = cursorPos.x + buttonSize.x;
			buttonEnd.y = cursorPos.y + buttonSize.y;

			// setting texture to the backward button for hovering and normal state
			if (ImGui::IsMouseHoveringRect(buttonStart, buttonEnd)) {
				ImU32 buttonColor = ImGui::IsMouseDown(0) ? buttonActiveColor : buttonHoverColor;
				drawList->AddRectFilled(buttonStart, buttonEnd, buttonColor, 10.0f);
			}

			ImTextureID backwardIconId = (ImTextureID)(intptr_t)m_ForwardIcon->GetRendererId();
			drawList->AddImage(backwardIconId, buttonStart, buttonEnd, { 0, 1 }, { 1, 0 });
		}
		cursorPos.x += buttonSize.x + 7.0f;
		
		// button for refresh
		{
			ImGui::SetCursorScreenPos(cursorPos);
			if (ImGui::InvisibleButton("refreshButton", buttonSize)) {
				FetchCurrentDirectoryContent();
			}

			buttonStart = cursorPos;
			buttonEnd.x = cursorPos.x + buttonSize.x;
			buttonEnd.y = cursorPos.y + buttonSize.y;

			// setting texture to the backward button for hovering and normal state
			if (ImGui::IsMouseHoveringRect(buttonStart, buttonEnd)) {
				ImU32 buttonColor = ImGui::IsMouseDown(0) ? buttonActiveColor : buttonHoverColor;
				drawList->AddRectFilled(buttonStart, buttonEnd, buttonColor, 10.0f);
			}

			ImTextureID backwardIconId = (ImTextureID)(intptr_t)m_RefreshIcon->GetRendererId();
			drawList->AddImage(backwardIconId, buttonStart, buttonEnd, { 0, 1 }, { 1, 0 });
		}
		cursorPos.x += buttonSize.x + 10.0f;

		// displaying current working directory for ContentBrowserPanel
		std::string directory = m_CurrentDirectory.string();
		ImFont* directoryFont = FontManager::GetFont(FontWeight::Regular, 22);
		drawList->AddText(directoryFont, directoryFont->FontSize, cursorPos, 0xFFFFFFFF, directory.c_str(), NULL);
	}

	void ContentBrowserPanel::FetchCurrentDirectoryContent() {
		m_CurrentDirectoryContent.clear();

		for (const auto& directory : std::filesystem::directory_iterator(m_CurrentDirectory)) {
			m_CurrentDirectoryContent.emplace_back(directory);
		}
	}

	void ContentBrowserPanel::ForwardNavigationCallback() {
		// if there exist any directory in the forward Nav history setting that to the m_CurrentDirectory after
		// putting m_CurrentDirectory in the backward history and fetching the updated directory content 
		if (!m_ForwardNavigationHistory.empty()) {
			m_BackwardNavigationHistory.emplace(m_CurrentDirectory);

			m_CurrentDirectory = m_ForwardNavigationHistory.top();
			m_ForwardNavigationHistory.pop();

			FetchCurrentDirectoryContent();
		}
	}

	void ContentBrowserPanel::BackwardNavigationCallback() {
		// if there exist any directory in the backward Nav history, setting that to the m_CurrentDirectory after
		// putting m_CurrentDirectory in the forward history and fetching the updated directory content 
		if (!m_BackwardNavigationHistory.empty()) {
			m_ForwardNavigationHistory.emplace(m_CurrentDirectory);

			m_CurrentDirectory = m_BackwardNavigationHistory.top();
			m_BackwardNavigationHistory.pop();

			FetchCurrentDirectoryContent();
		}
	}

}
