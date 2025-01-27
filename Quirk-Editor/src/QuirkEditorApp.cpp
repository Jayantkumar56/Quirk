
#include "Quirk.h"
#include "Core/Application/EntryPoint.h"
#include "EditorFrame.h"
#include "FontManager.h"
#include "Theme.h"


namespace Quirk {

	class QuirkEditorApp : public Application {
	public:
		QuirkEditorApp(): 
				Application(L"Quirk Engine", RendererAPI::API::OpenGL)
		{
			WindowSpecification tempSpec{ "Quirk Editor", 1600, 900, 200, 50, true, false, true };
			AddFrame<EditorFrame>(tempSpec);

			FontManager::LoadFonts();
			Theme::SetTheme(ThemeName::DarkTheme);
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

