
#pragma once

extern Quirk::Application* Quirk::CreateApplication();

#ifdef QK_PLATFORM_WINDOWS


#include "Platform/Windows/WindowsWindow.h"

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd) {
	Quirk::WindowsWindow::Init(hInstance);
	Quirk::Application* app = Quirk::CreateApplication();

	app->Run();
	delete app;

	return 0;
}


#endif // QK_PLATFORM_WINDOWS


