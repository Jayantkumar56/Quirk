

#pragma once

#include <Quirk.h>

#include "Core/Assets/RenderAssets/Texture.h"
#include "Base/ProjectManager.h"

#include <filesystem>
#include <stack>

namespace QuirkEditor {

	class ContentBrowserPanel : public Quirk::Panel {
	public:
		ContentBrowserPanel() : 
            Quirk::Panel("Content Browser")
		{
			m_CurrentDirectory = Quirk::ProjectManager::GetActive()->GetAssetDirectory();
			FetchCurrentDirectoryContent();

			m_FolderIcon   = Quirk::Texture2D::Create("assets/Images/folder.png");
			m_FileIcon     = Quirk::Texture2D::Create("assets/Images/file.png");
			m_BackwardIcon = Quirk::Texture2D::Create("assets/Images/backward_arrow.png");
			m_ForwardIcon  = Quirk::Texture2D::Create("assets/Images/forward_arrow.png");
			m_RefreshIcon  = Quirk::Texture2D::Create("assets/Images/refresh.png");
		}

		virtual void SetImguiProperties() override;
		virtual void OnImguiUiUpdate()    override;

	private:
		void DrawMenuBar();
		void FetchCurrentDirectoryContent();
		void ForwardNavigationCallback();
		void BackwardNavigationCallback();

	private:
		// TO DO: these resources should be managed automatically by resource manager
		Quirk::Ref<Quirk::Texture2D> m_FileIcon;
		Quirk::Ref<Quirk::Texture2D> m_FolderIcon;
		Quirk::Ref<Quirk::Texture2D> m_BackwardIcon;
		Quirk::Ref<Quirk::Texture2D> m_ForwardIcon;
		Quirk::Ref<Quirk::Texture2D> m_RefreshIcon;

		std::filesystem::path m_CurrentDirectory;
		std::vector<std::filesystem::path> m_CurrentDirectoryContent;
		std::stack<std::filesystem::path>  m_ForwardNavigationHistory;
		std::stack<std::filesystem::path>  m_BackwardNavigationHistory;
	};

}
