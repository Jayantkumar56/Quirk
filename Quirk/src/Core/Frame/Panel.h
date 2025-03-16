

#pragma once

#include "FrameBase.h"
#include "Window.h"
#include "Core/Input/Events.h"

namespace Quirk {

	class FrameManager;

	class Panel {
		friend class Frame;
		friend class FrameManager;

	public:
		Panel(const char* title, ImGuiWindowFlags flags = 0) : m_Title(title), m_WindowFlags(flags) { }
		virtual ~Panel() = default;

		virtual void OnUpdate()             { }
		virtual void OnImguiUiUpdate()      { }
		virtual bool OnEvent(Event& event)  { return false; }

		// SetImguiWindowProperties is called before OnImguiUiUpdate
		// thus could be used to set Imgui properties which are set before calling ImGui::Begin()
		virtual void SetImguiProperties()   { }

		// UnSetImguiProperties is called after OnImguiUiUpdate
		// thus could be used to unset Imgui properties which are set in SetImguiProperties()
		virtual void UnSetImguiProperties() { }

		// since Panels are managed by the frame manager thus by just setting m_PanelOpen to false
		// will make the frame manager close this Panels
		inline void CloseFrame() noexcept { m_PanelOpen = false; }

		std::string_view  GetTitle()           noexcept { return m_Title;                    }
		Window&           GetWindow()          noexcept { return m_ParentFrame->GetWindow(); }
		inline FrameBase* GetParentFrame()     noexcept { return m_ParentFrame;              }
		inline void       SetWindowFlags(ImGuiWindowFlags flags) noexcept { m_WindowFlags = flags; }

	private:
		inline void OnUiUpdate() {
			SetImguiProperties();

			ImGui::Begin(m_Title, &m_PanelOpen, m_WindowFlags);
			OnImguiUiUpdate();
			ImGui::End();

			UnSetImguiProperties();
		}

	private:
		bool m_PanelOpen = true;

		ImGuiWindowFlags m_WindowFlags;

		// title for the ImGui Window
		const char* m_Title;

		// to communicate with the parent Frame obj which manages this panel
		FrameBase* m_ParentFrame = nullptr;
	};

	template <typename T>
	concept PanelType = std::derived_from<T, Panel>;

}
