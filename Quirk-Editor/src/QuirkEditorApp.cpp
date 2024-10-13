
#include "Quirk.h"
#include "Core/Application/EntryPoint.h"
#include "EditorLayer.h"
#include "FontManager.h"
#include "Theme.h"

namespace Quirk {

	class QuirkEditorApp : public Application {
	public:
		QuirkEditorApp(): 
				Application(L"Quirk Engine Editor")
		{
			ImguiUI::Init();
			ImguiUI::EnableDocking();

			FontManager::LoadFonts();
			Theme::SetTheme(ThemeName::DarkTheme);

			GetWindow().SetVSync(true);

			PushLayer(&m_EditorLayer);
		}

		~QuirkEditorApp() {
			ImguiUI::Terminate();
		}

	private:
		EditorLayer m_EditorLayer;
	};


	// LaunchApp() must be defined in the application
	// Create app object inside this function and call Run method
	void LaunchApp() {
		QuirkEditorApp app;
		app.Run();
	}

}

