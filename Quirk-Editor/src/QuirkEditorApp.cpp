
#include "Quirk.h"
#include "Core/Application/EntryPoint.h"
#include "EditorLayer.h"

namespace Quirk {

	class QuirkEditorApp : public Application {
	public:
		QuirkEditorApp(): 
				Application(L"Quirk Engine Editor")
		{
			PushLayer(&m_EditorLayer);

			ImguiUI::Init();
			ImguiUI::EnableDocking();

			GetWindow().SetVSync(true);
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

