

#pragma once

#include <Quirk.h>

#include "Core/Assets/RenderAssets/Texture.h"
#include "EditorCore/ProjectManager.h"

#include <filesystem>
#include <stack>

namespace Quirk {

	class ContentBrowserPanel : public Panel {
	public:
		ContentBrowserPanel() : 
				Panel("Content Browser")
		{
			m_CurrentDirectory = ProjectManager::GetActive()->GetAssetDirectory();
			FetchCurrentDirectoryContent();

			m_FolderIcon   = Texture2D::Create("assets/Images/folder.png");
			m_FileIcon     = Texture2D::Create("assets/Images/file.png");
			m_BackwardIcon = Texture2D::Create("assets/Images/backward_arrow.png");
			m_ForwardIcon  = Texture2D::Create("assets/Images/forward_arrow.png");
			m_RefreshIcon  = Texture2D::Create("assets/Images/refresh.png");
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
		Ref<Texture2D> m_FileIcon;
		Ref<Texture2D> m_FolderIcon;
		Ref<Texture2D> m_BackwardIcon;
		Ref<Texture2D> m_ForwardIcon;
		Ref<Texture2D> m_RefreshIcon;

		std::filesystem::path m_CurrentDirectory;
		std::vector<std::filesystem::path> m_CurrentDirectoryContent;
		std::stack<std::filesystem::path>  m_ForwardNavigationHistory;
		std::stack<std::filesystem::path>  m_BackwardNavigationHistory;
	};

}
