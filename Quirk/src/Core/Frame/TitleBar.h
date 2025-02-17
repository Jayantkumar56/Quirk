

#pragma once

#include "FrameBase.h"
#include "Window.h"
#include "Core/Input/Events.h"
#include "Core/Imgui/ImguiUI.h"

namespace Quirk {

	class FrameManager;

	class TitleBar {
		friend class FrameManager;
		friend class Frame;

	public:
		TitleBar()          = default;
		virtual ~TitleBar() = default;

		virtual void OnImguiUiUpdate()     = 0;
		virtual bool OnEvent(Event& event) = 0;

		// SetImguiWindowProperties is called before OnImguiUiUpdate
		// thus could be used to set Imgui properties which are set before calling ImGui::Begin()
		virtual void SetImguiProperties() { }

		// UnSetImguiProperties is called after OnImguiUiUpdate
		// thus could be used to unset Imgui properties which are set in SetImguiProperties()
		virtual void UnSetImguiProperties() { }

		Window& GetWindow()                noexcept { return m_ParentFrame->GetWindow(); }
		inline FrameBase* GetParentFrame() noexcept { return m_ParentFrame;              }

		inline void SetCursorOverMinimiseButton (bool toggle) noexcept { GetWindow().SetCursorOverMinimiseButton(toggle); }
		inline void SetCursorOverMaximiseButton (bool toggle) noexcept { GetWindow().SetCursorOverMaximiseButton(toggle); }
		inline void SetCursorOverCloseButton    (bool toggle) noexcept { GetWindow().SetCursorOverCloseButton(toggle);    }

	private:
		inline void OnUiUpdate() {
			SetImguiProperties();

			if (ImGui::BeginMainMenuBar()) {
				OnImguiUiUpdate();

				// telling window if it can move with cursor (when dragging titlebar with mouse)
				// should only set true in requred condition since resetting is done 
				// in every cycle in the OnUpdate() of the FrameManager
				if (ImGui::IsWindowHovered() && !ImGui::IsAnyItemHovered())
					GetWindow().SetCanMoveWithCursor(true);

				ImGui::EndMainMenuBar();
			}

			UnSetImguiProperties();
		}

	private:
		// to communicate with the parent Frame obj which manages this titlebar
		FrameBase* m_ParentFrame = nullptr;
	};

	template <typename T>
	concept TitleBarType = std::derived_from<T, TitleBar>;

}
