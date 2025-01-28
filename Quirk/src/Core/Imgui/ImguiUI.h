

#pragma once

// TO DO: remove obsolete function calls with this
//#define IMGUI_DISABLE_OBSOLETE_FUNCTIONS

#include "imgui.h"
#include "imgui_internal.h"

namespace Quirk {

	class ImguiUI {
	public:
#ifdef QK_PLATFORM_WINDOWS
		struct ContextData { HDC DeviceContext = nullptr; };
#endif // QK_PLATFORM_WINDOWS

		void Init(Window& window, const Scope<GraphicalContext>& context);
		void Terminate();

		void Begin() const;
		void End(const Scope<GraphicalContext>& context) const;

		// should be called at some interval to update the imgui windows
		void UpdateViewPorts() const;

		inline void EnableDocking()  { m_DockingEnabled = true;  }
		inline void DisableDocking() { m_DockingEnabled = false; }

		inline void MakeCurrentImguiUIContext() { ImGui::SetCurrentContext(m_Context); }

	private:
		void InitForOpenGL(Window& window, const Scope<GraphicalContext>& context);

	private:
		bool m_DockingEnabled = true;

		// stores ImguiContext (must set before calling imgui functions)
		ImGuiContext* m_Context = nullptr;
	};

}

