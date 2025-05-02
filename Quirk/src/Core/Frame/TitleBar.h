

#pragma once

#include "FrameBase.h"
#include "Window.h"
#include "Core/Input/Events.h"
#include "Core/Imgui/ImguiUI.h"

namespace Quirk {

	class TitleBar {
		friend class FrameManager;

	public:
		TitleBar()          = default;
		virtual ~TitleBar() = default;

		virtual void OnImguiUiUpdate()     { }
		virtual bool OnEvent(Event& event) { return false; }

		// SetImguiWindowProperties is called before OnImguiUiUpdate
		// thus could be used to set Imgui properties which are set before calling ImGui::Begin()
		virtual void SetImguiProperties() { }

		// UnSetImguiProperties is called after OnImguiUiUpdate
		// thus could be used to unset Imgui properties which are set in SetImguiProperties()
		virtual void UnSetImguiProperties() { }

		Window& GetWindow() noexcept { return m_ParentFrame->GetWindow(); }

        template<FrameType T>
        inline T* GetParentFrameAs() noexcept { return static_cast<T*>(m_ParentFrame); }

		inline void SetCursorOverMinimiseButton (bool toggle) noexcept { GetWindow().SetCursorOverMinimiseButton(toggle); }
		inline void SetCursorOverMaximiseButton (bool toggle) noexcept { GetWindow().SetCursorOverMaximiseButton(toggle); }
		inline void SetCursorOverCloseButton    (bool toggle) noexcept { GetWindow().SetCursorOverCloseButton(toggle);    }

	public:
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

	public:
		// to communicate with the parent Frame obj which manages this titlebar
		FrameBase* m_ParentFrame = nullptr;
	};

	template <typename T>
	concept TitleBarType = std::derived_from<T, TitleBar>;


    class TitleBarManager {
    public:
        virtual ~TitleBarManager() { delete m_TitleBar; }

        // lifetime of the titlebar is managed by the frame
        template<TitleBarType T, typename ...Args>
        inline void SetTitleBar(FrameBase* frame, Args&& ... args) {
            // TODO: think about this static_cast
            m_TitleBar = static_cast<T*>(new T(std::forward<Args>(args)...));
            m_TitleBar->m_ParentFrame = frame;
        }

        inline void UpdateTitleBarUI() {
            m_TitleBar->OnUiUpdate();
        }

        inline bool TitleBarHandleEvents(Event& event) {
            return m_TitleBar->OnEvent(event);
        }

    private:
        TitleBar* m_TitleBar = nullptr;
    };

}
