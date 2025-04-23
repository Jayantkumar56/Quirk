

#pragma once

#include "TitleBar.h"
#include "Panel.h"
#include "FrameBase.h"
#include "FontManager.h"

#include "FrameTraits.h"

namespace Quirk {

    template<PanelPolicy PPanel, TitleBarPolicy PTitleBar>
	class Frame : 
            public FrameBase, 
            public EnumPolicyToType<PPanel>::Type,
            public EnumPolicyToType<PTitleBar>::Type
    {
        using PanelManagerType    = EnumPolicyToType<PPanel>::Type;
        using TitleBarManagerType = EnumPolicyToType<PTitleBar>::Type;

	public:
		Frame(WindowSpecification& spec) : FrameBase(spec) {
			m_ImguiUI.Init(GetWindow(), GetGraphicalContext());
		}

		inline void MakeContextCurrent() noexcept override {
            GetGraphicalContext()->MakeContextCurrent();
			m_ImguiUI.MakeImguiUIContextCurrent();
		}

    private:
        virtual void UpdateFrame() override {
            GetWindow().OnUpdate();
            m_ImguiUI.UpdateViewPorts();
            OnUpdate();

            if constexpr (PPanel == PanelPolicy::Enabled) {
                PanelManagerType::UpdatePanels();
            }
        }

        virtual void UpdateFrameUI() override {
            // updating imgui ui of the current frame and it's panels
            m_ImguiUI.Begin();

            // resetting if the cursor is hovering over titlebar
            // thus it should be set by the titlebar in every frame 
            // titlebar should only set true in the requred condition
            GetWindow().SetCanMoveWithCursor(false);

            OnImguiUiUpdate();

            if constexpr (PTitleBar == TitleBarPolicy::Enabled) {
                TitleBarManagerType::UpdateTitleBarUI();
            }

            if constexpr (PPanel == PanelPolicy::Enabled) {
                PanelManagerType::UpdatePanelsUI();
            }

            m_ImguiUI.End(GetGraphicalContext());
        }

        virtual bool HandleEvent(Event& event) override {
            OnEvent(event);

            if constexpr (PTitleBar == TitleBarPolicy::Enabled) {
                TitleBarManagerType::TitleBarHandleEvents(event);
            }

            if constexpr (PPanel == PanelPolicy::Enabled) {
                PanelManagerType::HandlePanelsEvent(event);
            }

            return false;
        }

	private:
		ImguiUI m_ImguiUI;
	};

}
