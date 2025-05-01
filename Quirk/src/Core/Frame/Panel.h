

#pragma once

#include "FrameBase.h"
#include "Window.h"
#include "Core/Input/Events.h"

#include "imgui.h"

namespace Quirk {

	class Panel {
        friend class PanelManager;

	public:
		Panel(const char* title, ImGuiWindowFlags flags = 0) : m_Title(title), m_WindowFlags(flags) { }
		virtual ~Panel() = default;

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
		inline void CloseFrame() noexcept { m_PanelOpen = false; }

		std::string_view  GetTitle()           noexcept { return m_Title;                    }
		Window&           GetWindow()          noexcept { return m_ParentFrame->GetWindow(); }
		inline FrameBase* GetParentFrame()     noexcept { return m_ParentFrame;              }
		inline void       SetWindowFlags(ImGuiWindowFlags flags) noexcept { m_WindowFlags = flags; }

	private:
		inline void UpdateUi() {
            SetUiProperties();

			ImGui::Begin(m_Title, &m_PanelOpen, m_WindowFlags);
            OnUiUpdate();
			ImGui::End();

            UnsetUiProperties();
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

    class PanelManager {
    public:
        virtual ~PanelManager() {
            for (size_t i = 0; i < m_Panels.size(); ++i)
                delete m_Panels[i];
        }

        // lifetime of the panel is managed by the frame
        template<PanelType P, typename ...Args>
        inline void AddPanel(FrameBase* frame, Args&& ... args) {
            P* panel = new P(std::forward<Args>(args)...);
            panel->m_ParentFrame = frame;
            m_Panels.push_back(static_cast<Panel*>(panel));
        }

        inline Panel* GetPanel(const std::string_view panelName) {
            for (auto panel : m_Panels) {
                if (panelName == panel->GetTitle())
                    return panel;
            }

            QK_WARN("Specified Panel \"{0}\" does not exist!", panelName);
            return nullptr;
        }

        inline void UpdatePanels() {
            for (size_t i = 0; i < m_Panels.size(); ++i) {
                if (!m_Panels[i]->m_PanelOpen) {
                    delete m_Panels[i];
                    m_Panels.erase(m_Panels.begin() + i--);
                    continue;
                }

                m_Panels[i]->OnUpdate();
            }
        }

        inline void UpdatePanelsUI() {
            for (size_t j = 0; j < m_Panels.size(); ++j) {
                m_Panels[j]->UpdateUi();
            }
        }

        inline bool HandlePanelsEvent(Event& event) {
            for (auto panel : m_Panels)
                panel->OnEvent(event);

            return false;
        }

    private:
        std::vector<Panel*> m_Panels;
    };

}
