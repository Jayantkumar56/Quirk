

#pragma once

#include "Core/Input/Events.h"

#include "imgui.h"

namespace Quirk {

	class Panel {
	public:
		Panel(std::string title, ImGuiWindowFlags flags = 0) noexcept : 
                m_Title       ( std::move(title) ),
                m_WindowFlags ( flags            )
        {}

		virtual ~Panel() noexcept = default;

		virtual void OnUpdate()            { }
		virtual void OnUiUpdate()          { }
		virtual bool OnEvent(Event& event) { return false; }

		// SetImguiWindowProperties is called before OnImguiUiUpdate
		// thus could be used to set Imgui properties which are set before calling ImGui::Begin()
		virtual void SetUiProperties()   { }

		// UnSetImguiProperties is called after OnImguiUiUpdate
		// thus could be used to unset Imgui properties which are set in SetImguiProperties()
		virtual void UnsetUiProperties() { }

		// since Panels are managed by the frame manager thus by just setting m_PanelOpen to false
		// will make the frame manager close this Panels
		inline void             CloseFrame()           noexcept { m_PanelOpen = false;  }
        inline bool             IsOpen()         const noexcept { return m_PanelOpen;   }
		inline std::string_view GetTitle()       const noexcept { return m_Title;       }
        inline ImGuiWindowFlags GetWindowFlags() const noexcept { return m_WindowFlags; }

		inline void SetWindowFlags(ImGuiWindowFlags flags) noexcept { m_WindowFlags = flags; }

	private:
		bool             m_PanelOpen = true;
		std::string      m_Title;
		ImGuiWindowFlags m_WindowFlags;
	};

	template <typename T>
	concept PanelType = std::derived_from<T, Panel>;

}
