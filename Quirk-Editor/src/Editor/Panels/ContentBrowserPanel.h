

#pragma once


#include "Base/ProjectManager.h"
#include "Editor/EditorFrame.h"

#include "Core/Frame/Panel.h"

#include <vector>
#include <stack>
#include <filesystem>

namespace QuirkEditor {

	class ContentBrowserPanel final : public Quirk::Panel {
	public:
		ContentBrowserPanel(EditorFrame* frame, std::filesystem::path assetDirectory) noexcept :
                Quirk::Panel       ( "Content Browser"         ),
                m_CurrentDirectory ( std::move(assetDirectory) ),
                m_EditorFrame      ( frame                     )
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

        Quirk::View<EditorFrame> m_EditorFrame;
	};

}
