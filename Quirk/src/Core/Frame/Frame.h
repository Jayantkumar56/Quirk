

#pragma once

#include "FrameBase.h"
#include "FrameTraits.h"

namespace Quirk {

    template<FrameFeature ...Features>
	class Frame : 
            public FrameBase,
            public InheritFromTypeList<InheritanceVisibility::Public, SortedFramePolicies_T<Features...>>
    {
        using PolicyList = SortedFramePolicies_T<Features...>;

        // all the policies are contained in a TypeList<PolicyType...>
        using Policies = InheritFromTypeList<InheritanceVisibility::Public, PolicyList>;

        static constexpr bool HaveWindowPolicy   = IsEnumValuePresent_V<FrameFeature::Window,       Features...>;
        static constexpr bool HaveImguiPolicy    = IsEnumValuePresent_V<FrameFeature::ImGuiContext, Features...>;
        static constexpr bool HavePanelPolicy    = IsEnumValuePresent_V<FrameFeature::Panels,       Features...>;
        static constexpr bool HaveTitleBarPolicy = IsEnumValuePresent_V<FrameFeature::TitleBar,     Features...>;

	public:
        Frame(const WindowSpecification& spec) :
                FrameBase(),
                Policies(
                    ConstructFromTypeList<
                        Policies, 
                        FramePolicyTupleGetterAdapter<Frame>::Getter,
                        PolicyList
                    >(*this, spec)
                )
        {}

		inline void MakeContextCurrent() noexcept final override {
            Policies::GetGraphicalContext()->MakeContextCurrent();

            if (HaveImguiPolicy) {
			    Policies::GetImguiContext().MakeImguiUIContextCurrent();
            }
		}

    private:
        virtual void UpdateFrame() final override {
            Policies::GetWindow().OnUpdate();

            if (HaveImguiPolicy) {
                Policies::GetImguiContext().UpdateViewPorts();
            }

            if constexpr (HavePanelPolicy) {
                Policies::UpdatePanels();
            }

            // updating the ui
            {
                RenderCommands::Clear();

                UpdateFrameUI();

                Policies::SwapBuffer();
            }
        }

        void UpdateFrameUI() {
            // updating imgui ui of the current frame and it's panels
            if (HaveImguiPolicy) {
                Policies::GetImguiContext().Begin();
            }

            // resetting if the cursor is hovering over titlebar
            // thus it should be set by the titlebar in every frame 
            // titlebar should only set true in the requred condition
            Policies::GetWindow().SetCanMoveWithCursor(false);

            OnImguiUiUpdate();

            if constexpr (HaveTitleBarPolicy) {
                Policies::UpdateTitleBarUI();
            }

            if constexpr (HavePanelPolicy) {
                Policies::UpdatePanelsUI();
            }

            if (HaveImguiPolicy) {
                Policies::GetImguiContext().End(Policies::GetGraphicalContext());
            }
        }

        virtual bool HandleEvent(Event& event) final override {
            OnEvent(event);

            if constexpr (HaveTitleBarPolicy) {
                Policies::TitleBarHandleEvents(event);
            }

            if constexpr (HavePanelPolicy) {
                Policies::HandlePanelsEvent(event);
            }

            return false;
        }
	};

}
