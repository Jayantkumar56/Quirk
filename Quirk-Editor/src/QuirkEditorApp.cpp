
#include "Quirk.h"
#include "Core/Application/EntryPoint.h"
#include "QuirkEditorApp.h"

#include "Editor/EditorFrame.h"
#include "Launcher/LauncherFrame.h"

#include "Theme.h"

namespace Quirk {

	// LaunchApp() must be defined in the application
	// Create app object inside this function and call Run method
	void LaunchApp() {
		QuirkEditorApp app;
		app.Run();
	}

	QuirkEditorApp::QuirkEditorApp() :
		Application(L"Quirk Engine", RendererAPI::API::OpenGL)
	{
		WindowSpecification tempSpec{
			.Title             {"Quirk Engine"},
			.Width             {1100},				   .Height    {780},
			.PosX              {200},				   .PosY      {50},
			.VSyncOn           {true},				   .Maximized {false},
			.CustomTitleBar    {true},
			.WindowBorderSizeX {0},				       .WindowBorderSizeY {0},
			.WindowBorderColor {12, 12, 12},
			.MinWidth          {1100},				   .MinHeight {780}
		};

		AddFrame<LauncherFrame>(tempSpec);
	}

	void QuirkEditorApp::LaunchEditor() {
		WindowSpecification tempSpec{
			.Title             {"Quirk Editor"},
			.Width             {1600},				   .Height    {900},
			.PosX              {200},				   .PosY      {50},
			.VSyncOn           {true},				   .Maximized {true},
			.CustomTitleBar    {true},
			.WindowBorderSizeX {0},				       .WindowBorderSizeY {0},
			.WindowBorderColor {12, 12, 12},
			.MinWidth          {1600},				   .MinHeight {900}
		};

		// AddFrame adds the frame and makes that context to be current
		Application::Get().AddFrame<EditorFrame>(tempSpec);
	}

}

