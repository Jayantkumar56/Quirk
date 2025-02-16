

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
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, { 0.0f, 8.0f });
	}

	void ContentBrowserPanel::UnSetImguiProperties() {
		ImGui::PopStyleVar();
	}

	void ContentBrowserPanel::OnImguiUiUpdate() {
		bool updatedCurrentDirectory = false;
		
		if (ImGui::BeginMenuBar()) {
			ImTextureID backwardIconId = (ImTextureID)(intptr_t)m_BackwardIcon->GetRendererId();
			if (ImGui::ImageButton("backwardButton", backwardIconId, { 25.0f, 25.0f }, { 0, 1 }, { 1, 0 })) {
				if (!m_BackwardNavigationHistory.empty()) {
					m_ForwardNavigationHistory.emplace(m_CurrentDirectory);

					m_CurrentDirectory = m_BackwardNavigationHistory.top();
					m_BackwardNavigationHistory.pop();

					updatedCurrentDirectory = true;
				}
			}

			ImTextureID forwardIconId = (ImTextureID)(intptr_t)m_ForwardIcon->GetRendererId();
			if (ImGui::ImageButton("forwardButton", forwardIconId, { 25.0f, 25.0f }, { 0, 1 }, { 1, 0 })) {
				if (!m_ForwardNavigationHistory.empty()) {
					m_BackwardNavigationHistory.emplace(m_CurrentDirectory);

					m_CurrentDirectory = m_ForwardNavigationHistory.top();
					m_ForwardNavigationHistory.pop();

					updatedCurrentDirectory = true;
				}
			}

			ImTextureID refresIconId = (ImTextureID)(intptr_t)m_RefreshIcon->GetRendererId();
			if (ImGui::ImageButton("refreshButton", refresIconId, { 25.0f, 25.0f }, { 0, 1 }, { 1, 0 })) {
				FetchCurrentDirectoryContent();
			}

			ImGui::Text(m_CurrentDirectory.string().c_str());

			ImGui::EndMenuBar();
		}

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

	void ContentBrowserPanel::FetchCurrentDirectoryContent() {
		m_CurrentDirectoryContent.clear();

		for (const auto& directory : std::filesystem::directory_iterator(m_CurrentDirectory)) {
			m_CurrentDirectoryContent.emplace_back(directory);
		}
	}

}
