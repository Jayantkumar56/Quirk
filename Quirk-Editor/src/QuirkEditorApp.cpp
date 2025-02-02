
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
			WindowSpecification tempSpec{ "Quirk Editor", 1600, 900, 200, 50, true, false, true, 4, 4, {12, 12, 12} };

			tempSpec.Width  = 1600;
			tempSpec.Height = 900;
			tempSpec.MinWidth  = tempSpec.Width;
			tempSpec.MinHeight = tempSpec.Height;
			AddFrame<EditorFrame>(tempSpec);
			Theme::SetTheme(ThemeName::DarkTheme);

			// NOTE: for testing of multiple frames
			/*tempSpec.Width     = 1100;
			tempSpec.Height    = 780;
			tempSpec.MinWidth  = tempSpec.Width;
			tempSpec.MinHeight = tempSpec.Height;
			AddFrame<LauncherFrame>(tempSpec);*/
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

