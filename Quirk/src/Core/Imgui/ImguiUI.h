

#pragma once

#include "Core/Application/Window.h"

// TO DO: remove obsolete function calls with this
//#define IMGUI_DISABLE_OBSOLETE_FUNCTIONS

#ifdef QK_PLATFORM_WINDOWS
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif // WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include "wglext.h"
#include "backends/imgui_impl_win32.h"
#endif // QK_PLATFORM_WINDOWS

#include "imgui.h"
#include "imgui_internal.h"
#include "backends/imgui_impl_opengl3.h"

namespace Quirk { namespace ImguiUI {
	struct ContextData {
		HDC DeviceContext;
		ContextData() : DeviceContext(nullptr) {}
	};

	void EnableDocking();
	void DisableDocking();

	void Init(Window& window);
	void Terminate();

	void Begin();
	void End();
}}

