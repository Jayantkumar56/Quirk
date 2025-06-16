

#pragma once

#include "Core/Utility/View.h"

// TODO: remove obsolete function calls with this
//#define IMGUI_DISABLE_OBSOLETE_FUNCTIONS

#include "imgui.h"
#include "imgui_internal.h"
#include "misc/cpp/imgui_stdlib.h"


namespace Quirk {

	class Window;

	namespace RHI {
		class GraphicalContext;
	}

	class ImguiContext {
	public:
#ifdef QK_PLATFORM_WINDOWS
		struct ContextData { HDC DeviceContext = nullptr; };
#endif // QK_PLATFORM_WINDOWS

		ImguiContext(View<Window> window, ConstView<RHI::GraphicalContext> context) noexcept;
		~ImguiContext() noexcept;

		void Begin();
		void End();

		// should be called at some interval to update the imgui windows
		void UpdateViewPorts();

		inline void MakeImguiContextCurrent() noexcept { ImGui::SetCurrentContext(m_ImguiContext); }

	private:
		void InitForOpenGL(View<Window> window, ConstView<RHI::GraphicalContext> context);

	private:
		ImGuiContext* m_ImguiContext = nullptr;
		ConstView<RHI::GraphicalContext> m_GraphicalContext;
	};

}

