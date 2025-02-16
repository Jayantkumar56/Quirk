

#pragma once

#include "Window.h"
#include "Core/Input/Events.h"


namespace Quirk {

	class Frame;
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

		Window&       GetWindow()                            noexcept;
		inline Frame* GetParentFrame()                       noexcept { return m_ParentFrame;  }
		inline void   SetWindowFlags(ImGuiWindowFlags flags) noexcept { m_WindowFlags = flags; }

	private:
		inline void OnUiUpdate() {
			SetImguiProperties();

			ImGui::Begin(m_Title, (bool*)0, m_WindowFlags);
			OnImguiUiUpdate();
			ImGui::End();

			UnSetImguiProperties();
		}

	private:
		ImGuiWindowFlags m_WindowFlags;

		// title for the ImGui Window
		const char* m_Title;

		// to communicate with the parent Frame obj which manages this panel
		Frame* m_ParentFrame = nullptr;
	};

	template <typename T>
	concept PanelType = std::is_base_of<Panel, T>::value;

}
