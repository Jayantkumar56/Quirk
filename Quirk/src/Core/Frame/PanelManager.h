

#pragma once

#include "Panel.h"
#include "FrameInitContext.h"


namespace Quirk::Internals {

    class PanelManager {
    public:
        PanelManager(const FrameInitContext& initContext, auto& frame) noexcept {}

        template<PanelType P, typename ...Args>
        void AddPanel(Args&& ... args) {
            m_Panels.emplace_back(new P(std::forward<Args>(args)...));
        }

    protected:
        inline void UpdatePanels() {
            for (size_t i = 0; i < m_Panels.size(); ++i) {
                if (!m_Panels[i]->IsOpen()) {
                    m_Panels.erase(m_Panels.begin() + i--);
                    continue;
                }

                m_Panels[i]->OnUpdate();
            }
        }

        inline void UpdatePanelsUI() {
            for (auto& panel : m_Panels) {
                bool isPanelOpen = panel->IsOpen();

                panel->SetUiProperties();

                ImGui::Begin(panel->GetTitle().data(), &isPanelOpen, panel->GetWindowFlags());
                panel->OnUiUpdate();
                ImGui::End();

                panel->UnsetUiProperties();
            }
        }

        inline bool HandlePanelsEvent(Event& event) {
            for (auto& panel : m_Panels)
                panel->OnEvent(event);

            return false;
        }

    private:
        std::vector<Scope<Panel>> m_Panels;
    };

}
