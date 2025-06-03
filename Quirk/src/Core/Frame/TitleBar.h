

#pragma once

#include "FrameBase.h"
#include "Window.h"
#include "Core/Input/Events.h"
#include "Core/Imgui/ImguiUI.h"

namespace Quirk {

	class TitleBar {
        friend class TitleBarManager;

	public:
		virtual ~TitleBar() = default;

		virtual void OnImguiUiUpdate()     { }
		virtual bool OnEvent(Event& event) { return false; }

		// SetImguiWindowProperties is called before OnImguiUiUpdate
		// thus could be used to set Imgui properties which are set before calling ImGui::Begin()
		virtual void SetImguiProperties() { }

		// UnSetImguiProperties is called after OnImguiUiUpdate
		// thus could be used to unset Imgui properties which are set in SetImguiProperties()
		virtual void UnSetImguiProperties() { }

        Quirk::View<Window> GetWindow() const noexcept { return m_Window; }

		inline void SetCursorOverMinimiseButton (bool toggle) noexcept { m_Window->SetCursorOverMinimiseButton(toggle); }
		inline void SetCursorOverMaximiseButton (bool toggle) noexcept { m_Window->SetCursorOverMaximiseButton(toggle); }
		inline void SetCursorOverCloseButton    (bool toggle) noexcept { m_Window->SetCursorOverCloseButton(toggle);    }

	private:
		inline void OnUiUpdate() {
			SetImguiProperties();

			if (ImGui::BeginMainMenuBar()) {
				OnImguiUiUpdate();

				// telling window if it can move with cursor (when dragging titlebar with mouse)
				// should only set true in requred condition since resetting is done 
				// in every cycle in the OnUpdate() of the FrameManager
				if (ImGui::IsWindowHovered() && !ImGui::IsAnyItemHovered())
                    m_Window->SetCanMoveWithCursor(true);

				ImGui::EndMainMenuBar();
			}

			UnSetImguiProperties();
		}

	private:
        Quirk::View<Window> m_Window;
	};

	template <typename T>
	concept TitleBarType = std::derived_from<T, TitleBar>;

}
