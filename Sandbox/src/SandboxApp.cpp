
#include "Quirk.h"
#include "Core/Application/EntryPoint.h"

#include "Example3d.h"
#include "Example2D.h"

class SandboxApp : public Quirk::Application {
public:
	SandboxApp(): Application(L"Sandbox App") {
		PushLayer(&m_ExampleLayer);
		//PushOverlay(&m_ImguiLayer);

		//GetWindow().SetVSync(true);
	}

	~SandboxApp() {

	}

private:
	Example2DLayer m_ExampleLayer;
	//Quirk::ImguiUI m_ImguiLayer;
};


// Quirk::LaunchApp() must be defined in the application
// Create app object inside this function and call Run method
void Quirk::LaunchApp() {
	SandboxApp app;
	app.Run();
}

