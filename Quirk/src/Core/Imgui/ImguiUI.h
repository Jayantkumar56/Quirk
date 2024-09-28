

#pragma once

#include "Core/Application/Layer.h"

#include "imgui.h"
#include "backends/imgui_impl_win32.h"
#include "backends/imgui_impl_opengl3.h"

#ifdef QK_PLATFORM_WINDOWS
#define WIN32_LEAN_AND_MEAN
#include<Windows.h>
#endif // QK_PLATFORM_WINDOWS


namespace Quirk {

	class  ImguiUI {
		struct ContextData {
			HDC DeviceContext;
			ContextData() : DeviceContext(nullptr) {}
		};

	public:
		ImguiUI() = delete;
		~ImguiUI() = delete;

		static void Init() ;
		static void Terminate() ;

		static void Begin();
		static void End();

		static inline void EnableDocking() { s_DockingEnabled = true; }
		static inline void DisableDocking() { s_DockingEnabled = false; }

	private:
		static void CleanupDeviceWGL(HWND hWnd, ContextData* data);
		static bool CreateDeviceWGL(HWND hWnd, ContextData* data);

		static void HookRendererCreateWindow(ImGuiViewport* viewport);
		static void HookRendererDestroyWindow(ImGuiViewport* viewport);
		static void HookRendererSwapBuffers(ImGuiViewport* viewport, void*);
		static void HookPlatformRenderWindow(ImGuiViewport* viewport, void*);

	private:
		static ContextData s_MainWindowContextData;
		static HGLRC GLContext;
		static bool s_DockingEnabled;
	};

}


