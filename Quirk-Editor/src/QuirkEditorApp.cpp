
#include "Quirk.h"
#include "Core/Application/EntryPoint.h"
#include "EditorFrame.h"
#include "FontManager.h"
#include "Theme.h"

#include "glm/glm.hpp"

namespace Quirk {

	class QuirkEditorApp : public Application {
	public:
		QuirkEditorApp(): 
				Application(L"Quirk Engine Editor", RendererAPI::API::OpenGL)
		{
			//EnableImguiUI(true);
			//GetWindow().SetVSync(true);

			FontManager::LoadFonts();
			Theme::SetTheme(ThemeName::DarkTheme);

			//PushLayer(&m_EditorLayer);
			AddFrame(&m_EditorFrame);
		}

		~QuirkEditorApp() {
			//ImguiUI::Terminate();
		}

	private:
		EditorFrame m_EditorFrame;
	};


	// LaunchApp() must be defined in the application
	// Create app object inside this function and call Run method
	void LaunchApp() {
		QuirkEditorApp app;
		app.Run();
	}

}

