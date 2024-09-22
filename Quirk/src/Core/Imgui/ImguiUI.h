

#pragma once

#include "Core/Application/Layer.h"

#include "imgui.h"
#include "backends/imgui_impl_win32.h"
#include "backends/imgui_impl_opengl3.h"


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

	private:
		static void CleanupDeviceWGL(HWND hWnd, ContextData* data);
		static bool CreateDeviceWGL(HWND hWnd, ContextData* data);

		static void HookRendererCreateWindow(ImGuiViewport* viewport);
		static void HookRendererDestroyWindow(ImGuiViewport* viewport);
		static void HookRendererSwapBuffers(ImGuiViewport* viewport, void*);
		static void HookPlatformRenderWindow(ImGuiViewport* viewport, void*);

	private:
		static ContextData m_MainWindowContextData;
		static HGLRC GLContext;
	};

}


