
#include "Quirk.h"


class SandboxApp : public Quirk::Application {
public:
	SandboxApp() {

	}

	~SandboxApp() {

	}
};


// Quirk::LaunchApp() must be defined in the application
// Create app object inside this function and call Run method
void Quirk::LaunchApp() {
	SandboxApp app;
	app.Run();
}

