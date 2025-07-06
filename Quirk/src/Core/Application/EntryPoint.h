

#pragma once

namespace Quirk {
	// LaunchApp() must be defined in the application
	// Create app object inside this function and call Run method
	void LaunchApp();
}

// Enable dedicated NVIDIA GPU
//extern "C" {
//	_declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
//}

#ifdef QK_PLATFORM_WINDOWS

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd) {
	// Set binary directory as current working directory
	// Ensure consistent working directory across different build systems
	{
		// Get the full path to the executable
		wchar_t exePath[MAX_PATH];
		GetModuleFileNameW(NULL, exePath, MAX_PATH);

		std::filesystem::path exeDir = std::filesystem::path(exePath).parent_path();
		std::filesystem::current_path(exeDir);
	}

#ifdef QK_ENABLE_LOG
	Quirk::Log::Init();
#endif // QK_ENABLE_LOG

	Quirk::WindowsWindow::Init(hInstance);
	Quirk::LaunchApp();
	Quirk::WindowsWindow::Terminate();

	return 0;
}

#endif // QK_PLATFORM_WINDOWS


