

#pragma once

#include "Core/Utility/View.h"

// TODO: remove obsolete function calls with this
//#define IMGUI_DISABLE_OBSOLETE_FUNCTIONS

#include "Core/Frame/Window.h"

#include "imgui.h"
#include "imgui_internal.h"
#include "misc/cpp/imgui_stdlib.h"
#include "Platform/OpenGL/GraphicalContext.h"


namespace Quirk {

	class ImguiContext {
	public:
#ifdef QK_PLATFORM_WINDOWS
		struct ContextData { HDC DeviceContext = nullptr; };
#endif // QK_PLATFORM_WINDOWS

		void Init(View<Window> window, View<RHI::GraphicalContext> context);
		void Terminate();

		void Begin();
		void End();

		// should be called at some interval to update the imgui windows
		void UpdateViewPorts();

		inline void MakeImguiContextCurrent() noexcept { ImGui::SetCurrentContext(m_ImguiContext); }

	private:
		void InitForOpenGL(View<Window> window, View<RHI::GraphicalContext> context);

	private:
		ImGuiContext* m_ImguiContext = nullptr;
        View<RHI::GraphicalContext> m_GraphicalContext;
	};

}

