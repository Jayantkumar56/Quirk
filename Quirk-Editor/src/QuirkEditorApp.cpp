
#include "Quirk.h"
#include "Core/Application/EntryPoint.h"
#include "SceneViewport.h"
#include "SceneProperties.h"


namespace Quirk {

	class QuirkEditorApp : public Application {
	public:
		QuirkEditorApp(): 
				Application(L"Quirk Engine Editor"),
				m_SceneProperties(m_SceneViewport)
		{
			PushLayer(&m_SceneViewport);
			PushLayer(&m_SceneProperties);

			ImguiUI::Init();
			ImguiUI::EnableDocking();

			GetWindow().SetVSync(false);
		}

		~QuirkEditorApp() {
			ImguiUI::Terminate();
		}

	private:
		SceneViewport m_SceneViewport;
		SceneProperties m_SceneProperties;
	};


	// LaunchApp() must be defined in the application
	// Create app object inside this function and call Run method
	void LaunchApp() {
		QuirkEditorApp app;
		app.Run();
	}

}

