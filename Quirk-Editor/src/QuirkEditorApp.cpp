

#include "QkEditorpch.h"

#include "QuirkEditorApp.h"
#include "Launcher/LauncherFrame.h"
#include "Editor/EditorFrame.h"

#include "Core/Application/EntryPoint.h"

#include <vector>


namespace Quirk {

	// LaunchApp() must be defined in the application
	// Create app object inside this function and call Run method
	void LaunchApp() {
        QuirkEditor::QuirkEditorApp app;
        app.Init();
		app.Run();
        app.Terminate();
	}

}

namespace QuirkEditor {

	void QuirkEditorApp::LaunchEditor() {
		auto* frame = AddFrame<EditorFrame>(m_ProjManager.GetActive());
	}

    void QuirkEditorApp::Init() {
        m_ProjManager.LoadRecentProjectsList("RecentProjects.yaml");
		LoadFontFiles();

		AddFrame<LauncherFrame>(m_ProjManager);
    }

    void QuirkEditorApp::Terminate() {
        m_ProjManager.SaveRecentProjectsList("RecentProjects.yaml");
    }

    void QuirkEditorApp::LoadFontFiles() {
        using namespace Quirk;

		////FontManager::SetFontFileToFontWeight(FontWeight::Regular, std::string("assets/Fonts/Schibsted_Grotesk/static/SchibstedGrotesk-Regular.ttf"));
		//FontManager::SetFontFileToFontWeight(FontWeight::Regular,   "assets/Fonts/Schibsted_Grotesk/static/SchibstedGrotesk-Regular.ttf"  );
		//FontManager::SetFontFileToFontWeight(FontWeight::Medium,    "assets/Fonts/Schibsted_Grotesk/static/SchibstedGrotesk-Medium.ttf"   );
		//FontManager::SetFontFileToFontWeight(FontWeight::SemiBold,  "assets/Fonts/Schibsted_Grotesk/static/SchibstedGrotesk-SemiBold.ttf" );
		//FontManager::SetFontFileToFontWeight(FontWeight::Bold,      "assets/Fonts/Schibsted_Grotesk/static/SchibstedGrotesk-Bold.ttf"     );
		//FontManager::SetFontFileToFontWeight(FontWeight::ExtraBold, "assets/Fonts/Schibsted_Grotesk/static/SchibstedGrotesk-ExtraBold.ttf");
		//FontManager::SetFontFileToFontWeight(FontWeight::Black,     "assets/Fonts/Schibsted_Grotesk/static/SchibstedGrotesk-Black.ttf"    );

		//// EditorFrame related fonts 
		//FontManager::LoadFont(FontWeight::Regular,  22                     );
		//FontManager::LoadFont(FontWeight::Bold,     18                     );
		//FontManager::LoadFont(FontWeight::Regular,  16, "DragFloatValue"   );
		//FontManager::LoadFont(FontWeight::SemiBold, 20, "PropertyLabel"    );
		//FontManager::LoadFont(FontWeight::SemiBold, 22, "ComponentTreeNode");

		// LauncherFrame realated fonts
		//FontManager::LoadFont(FontWeight::Medium,  50);     // for Main Title (Quirk Game Engine)
		//FontManager::LoadFont(FontWeight::Medium,  29);		// for section title ()
		//FontManager::LoadFont(FontWeight::Medium,  25);
		//FontManager::LoadFont(FontWeight::Regular, 20);
		//FontManager::LoadFont(FontWeight::Regular, 23);
	}

}

