
#include "Quirk.h"
#include "Core/Application/EntryPoint.h"

#include "Example2D.h"


namespace Quirk {

	class QuirkEditorApp : public Application {
	public:
		QuirkEditorApp() {
			PushLayer(&m_ExampleLayer);
			ImguiUI::Init();

			//GetWindow().SetVSync(true);
		}

		~QuirkEditorApp() {
			ImguiUI::Terminate();
		}

	private:
		Example2DLayer m_ExampleLayer;
	};


	// LaunchApp() must be defined in the application
	// Create app object inside this function and call Run method
	void LaunchApp() {
		QuirkEditorApp app;
		app.Run();
	}

}

