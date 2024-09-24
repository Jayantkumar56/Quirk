
#include "Quirk.h"
#include "Core/Application/EntryPoint.h"
#include "Core/Imgui/ImguiUI.h"
#include "Example2D.h"
#include "SceneViewport.h"

namespace Quirk {

	class QuirkEditorApp : public Application {
	public:
		QuirkEditorApp(): Application(L"Quirk Engine Editor") {
			PushLayer(&m_SceneViewport);
			ImguiUI::Init();
			ImguiUI::EnableDocking();

			//GetWindow().SetVSync(true);
		}

		~QuirkEditorApp() {
			ImguiUI::Terminate();
		}

	private:
		SceneViewport m_SceneViewport;
	};


	// LaunchApp() must be defined in the application
	// Create app object inside this function and call Run method
	void LaunchApp() {
		QuirkEditorApp app;
		app.Run();
	}

}

