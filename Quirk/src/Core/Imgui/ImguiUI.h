

#pragma once

#include "Core/Application/Window.h"
#include "Core/Renderer/GraphicalContext.h"

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

namespace Quirk {

	class ImguiUI {
	public:
#ifdef QK_PLATFORM_WINDOWS
		struct ContextData {
			HDC DeviceContext = nullptr;
			ContextData() = default;
		};
#endif // QK_PLATFORM_WINDOWS

		void Init(Window& window, const Scope<GraphicalContext>& context);
		void Terminate();

		void Begin() const;
		void End(const Scope<GraphicalContext>& context);

		inline void EnableDocking()  { m_DockingEnabled = true;  }
		inline void DisableDocking() { m_DockingEnabled = false; }

		inline void SetCurrentImguiContext() { ImGui::SetCurrentContext(m_Context); }

	private:
		void InitForOpenGL(Window& window, const Scope<GraphicalContext>& context);

	private:
		bool m_DockingEnabled = true;

		// stores ImguiContext (must set before calling imgui functions)
		ImGuiContext* m_Context = nullptr;
	};

}

