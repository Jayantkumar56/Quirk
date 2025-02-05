
#include "Quirk.h"
#include "Core/Application/EntryPoint.h"

#include "Editor/EditorFrame.h"
#include "Launcher/LauncherFrame.h"

#include "Theme.h"

namespace Quirk {

	class QuirkEditorApp : public Application {
	public:
		QuirkEditorApp(): 
				Application(L"Quirk Engine", RendererAPI::API::OpenGL)
		{
			WindowSpecification tempSpec{
				.Title             {"Quirk Engine"},
				.Width             {1100},				   .Height    {780},
				.PosX              {200},				   .PosY      {50},
				.VSyncOn           {true},				   .Maximized {false},
				.CustomTitleBar    {true},
				.WindowBorderSizeX {4},				       .WindowBorderSizeY {4},
				.WindowBorderColor {12, 12, 12},
				.MinWidth          {1100},				   .MinHeight {780}
			};

			AddFrame<LauncherFrame>(tempSpec);

			/*tempSpec.Width  = 1600;
			tempSpec.Height = 900;
			tempSpec.MinWidth  = tempSpec.Width;
			tempSpec.MinHeight = tempSpec.Height;
			AddFrame<EditorFrame>(tempSpec);
			Theme::SetTheme(ThemeName::DarkTheme);*/
		}

		~QuirkEditorApp() {

		}	
	};


	// LaunchApp() must be defined in the application
	// Create app object inside this function and call Run method
	void LaunchApp() {
		QuirkEditorApp app;
		app.Run();
	}

}

