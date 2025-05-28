

#include "QkEditorpch.h"

#include "EditorFrame.h"
#include "EditorTitleBar.h"
#include "Panels/SceneViewportPanel.h"
#include "Panels/SceneHierarchyPanel.h"
#include "Panels/InspectorPanel/InspectorPanel.h"
#include "Panels/ContentBrowserPanel.h"

namespace QuirkEditor {

    void EditorFrame::Init() noexcept {
        Quirk::Renderer::InitRenderer();
		Quirk::Renderer2D::InitRenderer();

        m_Theme.SetTheme(ThemeName::DarkTheme);

		SetTitleBar<EditorTitleBar>(this);

		AddPanel<SceneViewportPanel> (this);
		AddPanel<SceneHierarchyPanel>(this);
		AddPanel<InspectorPanel>     (this);
		AddPanel<ContentBrowserPanel>(this, m_Project->GetAssetDirectory());
    }

}

