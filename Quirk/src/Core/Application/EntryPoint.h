

#pragma once

namespace Quirk {
	void LaunchApp();
}

#ifdef QK_PLATFORM_WINDOWS

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd) {
#ifdef QK_ENABLE_LOG
	Quirk::Log::Init();
#endif // QK_ENABLE_LOG

	Quirk::Window::Init(hInstance);
	Quirk::LaunchApp();

	return 0;
}

#endif // QK_PLATFORM_WINDOWS


