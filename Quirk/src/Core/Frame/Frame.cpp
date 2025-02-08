
#include "Qkpch.h"
#include "Frame.h"
#include "Core/Application/Application.h"
#include "Core/Input/ApplicationEvents.h"

namespace Quirk {

	Window& Quirk::TitleBar::GetWindow() noexcept {
		return m_ParentFrame->GetWindow();
	}

	Window& Panel::GetWindow() noexcept {
		return m_ParentFrame->GetWindow();
	}

	void FrameManager::UpdateFrames() {
		// if there are no frames in the application we can terminate the Application
		if (!m_Frames.size()) {
			WindowCloseEvent e;
			EventDispatcher::DispatchEvent(e);
		}

		for (size_t i = 0; i < m_Frames.size(); ++i) {
			auto frame = m_Frames[i];

			// setting graphical and imgui context for currrent frame
			frame->m_Context->MakeContextCurrent();
			frame->m_ImguiUI.MakeImguiUIContextCurrent();

			// if frame->m_Runing is set false that means the frame should be deleted
			// decremented i because the (i+1)th element will now be (i)th element which needs to be processed
			// continued this iteration as there might be no next element in the vector
			if (!frame->m_Running) {
				delete frame;
				m_Frames.erase(m_Frames.begin() + i--);
				continue;
			}

			// clearing the backbuffer
			RenderCommands::Clear();

			frame->m_Window.OnUpdate();
			frame->m_ImguiUI.UpdateViewPorts();
			frame->OnUpdate();

			for (auto& panel : frame->m_Panels)
				panel->OnUpdate();

			// updating imgui ui of the current frame and it's panels
			frame->m_ImguiUI.Begin();

			// resetting if the cursor is hovering over titlebar
			// thus it should be set by the titlebar in every frame 
			// titlebar should only set true in the requred condition
			frame->GetWindow().SetCanMoveWithCursor(false);

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

