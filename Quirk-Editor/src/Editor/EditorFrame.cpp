

#include "EditorFrame.h"
#include "EditorTitleBar.h"
#include "Panels/SceneViewportPanel.h"
#include "Panels/SceneHierarchyPanel.h"
#include "Panels/InspectorPanel/InspectorPanel.h"
#include "Panels/ContentBrowserPanel.h"
#include "Base/Reflection/ProjectManagerReflection.h"
#include "Core/AssetManager/EditorAssetManager.h"
#include "Core/Serialization/Serialization.h"


namespace QuirkEditor {

    void EditorFrame::Init() noexcept {
        m_Project->SetRHIFactory(GetRenderSystem()->GetFactory());

        m_Theme.SetTheme(ThemeName::DarkTheme);

        // adding titlebar and panels
        {
		    SetTitleBar<EditorTitleBar>(this);

		    AddPanel<SceneViewportPanel> (this);
		    AddPanel<SceneHierarchyPanel>(this);
		    AddPanel<InspectorPanel>     (this);
		    AddPanel<ContentBrowserPanel>(this, m_Project->GetAssetDirectory());
        }

        // loading fonts
        {
            using namespace Quirk;
            View<FontManager> fontManager = GetFontManager();

            fontManager->SetFontFileToFontWeight(
                FontWeight::Regular,
                "assets/Fonts/Schibsted_Grotesk/static/SchibstedGrotesk-Regular.ttf"
            );

		    fontManager->SetFontFileToFontWeight(
                FontWeight::Medium,
                "assets/Fonts/Schibsted_Grotesk/static/SchibstedGrotesk-Medium.ttf"
            );

		    fontManager->SetFontFileToFontWeight(
                FontWeight::SemiBold,
                "assets/Fonts/Schibsted_Grotesk/static/SchibstedGrotesk-SemiBold.ttf"
            );

		    fontManager->SetFontFileToFontWeight(
                FontWeight::Bold,
                "assets/Fonts/Schibsted_Grotesk/static/SchibstedGrotesk-Bold.ttf"
            );

		    fontManager->SetFontFileToFontWeight(
                FontWeight::ExtraBold,
                "assets/Fonts/Schibsted_Grotesk/static/SchibstedGrotesk-ExtraBold.ttf"
            );

		    fontManager->SetFontFileToFontWeight(
                FontWeight::Black,
                "assets/Fonts/Schibsted_Grotesk/static/SchibstedGrotesk-Black.ttf"
            );


            fontManager->LoadFont(FontWeight::Regular,  22                     );
		    fontManager->LoadFont(FontWeight::Bold,     18                     );
		    fontManager->LoadFont(FontWeight::Regular,  16, "DragFloatValue"   );
		    fontManager->LoadFont(FontWeight::SemiBold, 20, "PropertyLabel"    );
		    fontManager->LoadFont(FontWeight::SemiBold, 22, "ComponentTreeNode");
        }
    }

    void EditorFrame::Terminate() noexcept {
        Quirk::Serialization::Serialize(m_Project, m_Project->GetProjFilePath());
    }

}

