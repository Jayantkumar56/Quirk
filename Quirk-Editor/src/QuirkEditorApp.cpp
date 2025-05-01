

#include "QkEditorpch.h"

#include "Quirk.h"
#include "Core/Application/EntryPoint.h"
#include "QuirkEditorApp.h"

#include "Editor/EditorFrame.h"
#include "Launcher/LauncherFrame.h"

#include "Editor/Theme.h"

namespace Quirk {

	// LaunchApp() must be defined in the application
	// Create app object inside this function and call Run method
	void LaunchApp() {
        QuirkEditor::QuirkEditorApp app;
		app.Run();
	}

}

namespace QuirkEditor {


	QuirkEditorApp::QuirkEditorApp() :
		Application(L"Quirk Engine", Quirk::RendererAPI::API::OpenGL)
	{
		LoadFontFiles();

        Quirk::WindowSpecification tempSpec{
			.Title             {"Quirk Engine"},
			.Width             {1100},				   .Height    {780},
			.MinWidth          {1100},				   .MinHeight {780},
			.PosX              {200},				   .PosY      {50},
			.VSyncOn           {true},				   .Maximized {false},
			.CustomTitleBar    {true}
		};

		AddFrame<LauncherFrame>(tempSpec);

		// NOTE: Temporary            directly opening the editor
		//std::filesystem::path path = "../Example/Example.qkproj";
		//Project::Load<EditorAssetManager>(path);
		//LaunchEditor();
	}

	void QuirkEditorApp::LaunchEditor() {
        Quirk::WindowSpecification tempSpec{
			.Title             {"Quirk Editor"},
			.Width             {1600},				   .Height    {900},
			.MinWidth          {1600},				   .MinHeight {900},
			.PosX              {200},				   .PosY      {50},
			.VSyncOn           {true},				   .Maximized {true},
			.CustomTitleBar    {true}
		};

		// AddFrame adds the frame and makes that context to be current
		AddFrame<EditorFrame>(tempSpec);
	}

	void QuirkEditorApp::LoadFontFiles() {
        using namespace Quirk;

		//FontManager::SetFontFileToFontWeight(FontWeight::Regular, std::string("assets/Fonts/Schibsted_Grotesk/static/SchibstedGrotesk-Regular.ttf"));
		FontManager::SetFontFileToFontWeight(FontWeight::Regular,   "assets/Fonts/Schibsted_Grotesk/static/SchibstedGrotesk-Regular.ttf"  );
		FontManager::SetFontFileToFontWeight(FontWeight::Medium,    "assets/Fonts/Schibsted_Grotesk/static/SchibstedGrotesk-Medium.ttf"   );
		FontManager::SetFontFileToFontWeight(FontWeight::SemiBold,  "assets/Fonts/Schibsted_Grotesk/static/SchibstedGrotesk-SemiBold.ttf" );
		FontManager::SetFontFileToFontWeight(FontWeight::Bold,      "assets/Fonts/Schibsted_Grotesk/static/SchibstedGrotesk-Bold.ttf"     );
		FontManager::SetFontFileToFontWeight(FontWeight::ExtraBold, "assets/Fonts/Schibsted_Grotesk/static/SchibstedGrotesk-ExtraBold.ttf");
		FontManager::SetFontFileToFontWeight(FontWeight::Black,     "assets/Fonts/Schibsted_Grotesk/static/SchibstedGrotesk-Black.ttf"    );

		// EditorFrame related fonts 
		FontManager::LoadFont(FontWeight::Regular,  22                     );
		FontManager::LoadFont(FontWeight::Bold,     18                     );
		FontManager::LoadFont(FontWeight::Regular,  16, "DragFloatValue"   );
		FontManager::LoadFont(FontWeight::SemiBold, 20, "PropertyLabel"    );
		FontManager::LoadFont(FontWeight::SemiBold, 22, "ComponentTreeNode");

		// LauncherFrame realated fonts
		FontManager::LoadFont(FontWeight::Medium,  50);     // for Main Title (Quirk Game Engine)
		FontManager::LoadFont(FontWeight::Medium,  29);		// for section title ()
		FontManager::LoadFont(FontWeight::Medium,  25);
		FontManager::LoadFont(FontWeight::Regular, 20);
		FontManager::LoadFont(FontWeight::Regular, 23);
	}

}

