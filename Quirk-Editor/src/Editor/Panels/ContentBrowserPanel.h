

#pragma once

#include <Quirk.h>

#include <filesystem>
#include <stack>
#include "Core/Renderer/Texture.h"

namespace Quirk {

	class ContentBrowserPanel : public Panel {
	public:
		ContentBrowserPanel(const std::string& resourcePath) :
				m_BaseProjectDirectory(resourcePath)
		{
			QK_CORE_ASSERT(std::filesystem::exists(resourcePath), "Invalid file path provided");
			m_CurrentDirectory = resourcePath;
			FetchCurrentDirectoryContent();

			m_FolderIcon   = Texture2D::Create("assets/Images/folder.png");
			m_FileIcon     = Texture2D::Create("assets/Images/file.png");
			m_BackwardIcon = Texture2D::Create("assets/Images/icons8-backward-96.png");
			m_ForwardIcon  = Texture2D::Create("assets/Images/icons8-forward-96.png");
			m_RefreshIcon  = Texture2D::Create("assets/Images/refresh.png");
		}

		virtual void OnUpdate() override { }
		virtual bool OnEvent(Event& event) override { return false; }
		virtual void OnImguiUiUpdate() override;

	private:
		void FetchCurrentDirectoryContent();

	private:
		// TO DO: these resources should be managed automatically by resource manager
		Ref<Texture2D> m_FileIcon;
		Ref<Texture2D> m_FolderIcon;
		Ref<Texture2D> m_BackwardIcon;
		Ref<Texture2D> m_ForwardIcon;
		Ref<Texture2D> m_RefreshIcon;

		std::filesystem::path m_BaseProjectDirectory;
		std::filesystem::path m_CurrentDirectory;
		std::vector<std::filesystem::path> m_CurrentDirectoryContent;
		std::stack<std::filesystem::path>  m_ForwardNavigationHistory;
		std::stack<std::filesystem::path>  m_BackwardNavigationHistory;
	};

}
