

#pragma once

namespace Quirk {
	void LaunchApp();
}

// Enable dedicated NVIDIA GPU
//extern "C" {
//	_declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
//}

#ifdef QK_PLATFORM_WINDOWS

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd) {
#ifdef QK_ENABLE_LOG
	Quirk::Log::Init();
#endif // QK_ENABLE_LOG

	Quirk::WindowsWindow::Init(hInstance);
	Quirk::LaunchApp();

	return 0;
}

#endif // QK_PLATFORM_WINDOWS


