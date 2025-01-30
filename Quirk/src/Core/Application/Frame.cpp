
#include "Qkpch.h"
#include "Frame.h"


namespace Quirk {

	Window& Quirk::TitleBar::GetWindow() noexcept {
		return m_ParentFrame->GetWindow();
	}

	Window& Panel::GetWindow() noexcept {
		return m_ParentFrame->GetWindow();
	}

	void FrameManager::UpdateFrames() {
		for (auto& frame : m_Frames) {
			// setting graphical and imgui context for currrent frame
			frame->m_Context->MakeContextCurrent();
			frame->m_ImguiUI.MakeCurrentImguiUIContext();

			// clearing the backbuffer
			RenderCommands::Clear();

			frame->m_Window.OnUpdate();
			frame->m_ImguiUI.UpdateViewPorts();
			frame->OnUpdate();

			for (auto& panel : frame->m_Panels)
				panel->OnUpdate();

			// updating imgui ui of the current frame and it's panels
			frame->m_ImguiUI.Begin();

			frame->OnImguiUiUpdate();
			frame->m_TitleBar->OnUiUpdate();

			for (auto& panel : frame->m_Panels)
				panel->OnImguiUiUpdate();

			frame->m_ImguiUI.End(frame->m_Context);

			frame->SwapBuffer();
		}
	}

	bool FrameManager::HandleEvent(Event& event) {
		for (auto frame : m_Frames) {
			frame->OnEvent(event);
			frame->m_TitleBar->OnEvent(event);

			for (auto& panel : frame->m_Panels)
				panel->OnEvent(event);
		}

		return false;
	}
}

