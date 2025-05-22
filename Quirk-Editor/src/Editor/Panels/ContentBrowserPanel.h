

#pragma once

#include <Quirk.h>

#include "Core/Assets/RenderAssets/Texture.h"
#include "Base/ProjectManager.h"

#include <vector>
#include <stack>
#include <filesystem>

namespace QuirkEditor {

	class ContentBrowserPanel : public Quirk::Panel {
	public:
		ContentBrowserPanel(std::filesystem::path assetDirectory) noexcept : 
                Quirk::Panel       ( "Content Browser"         ),
                m_CurrentDirectory ( std::move(assetDirectory) )
		{
			FetchCurrentDirectoryContent();
		}

		virtual void SetUiProperties() override;
		virtual void OnUiUpdate()    override;

	private:
		void DrawMenuBar();
		void FetchCurrentDirectoryContent();
		void ForwardNavigationCallback();
		void BackwardNavigationCallback();

	private:
		std::filesystem::path m_CurrentDirectory;
		std::vector<std::filesystem::path> m_CurrentDirectoryContent;
		std::stack<std::filesystem::path>  m_ForwardNavigationHistory;
		std::stack<std::filesystem::path>  m_BackwardNavigationHistory;
	};

}
