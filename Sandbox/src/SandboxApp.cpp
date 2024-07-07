
#include "Quirk.h"


class SandboxApp : public Quirk::Application {
public:
	SandboxApp() {

	}

	~SandboxApp() {

	}

};

Quirk::Application* Quirk::CreateApplication(){
	return new SandboxApp();
}

