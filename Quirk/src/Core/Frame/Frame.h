

#pragma once

#include "FrameBase.h"
#include "FrameTraits.h"
#include "FrameInitContext.h"
#include "Core/Renderer/RenderCommands.h"

namespace Quirk::Internals {

    template<typename PolicyList>
    class FrameImpl {};

    template<typename ...Policy>
    class FrameImpl<TypeList<Policy...>> :
            public FrameBase,
            public Policy...
    {
        using GraphicalContextPolicy = GraphicalContextManager;

        using WindowPolicy   = FramePolicyType_T<FrameFeature::Window      >;
        using ImguiPolicy    = FramePolicyType_T<FrameFeature::ImGuiContext>;
        using PanelPolicy    = FramePolicyType_T<FrameFeature::Panels      >;
        using TitleBarPolicy = FramePolicyType_T<FrameFeature::TitleBar    >;

        static constexpr bool HaveWindowPolicy   = IsPresent_V<WindowPolicy,   Policy...>;
        static constexpr bool HaveImguiPolicy    = IsPresent_V<ImguiPolicy,    Policy...>;
        static constexpr bool HavePanelPolicy    = IsPresent_V<PanelPolicy,    Policy...>;
        static constexpr bool HaveTitleBarPolicy = IsPresent_V<TitleBarPolicy, Policy...>;

    public:
        FrameImpl(const FrameInitContext& initContext) noexcept :
                Policy(initContext, *this)...
        {}
        
        inline void MakeContextCurrent() noexcept final override {
            GraphicalContextPolicy::GetGraphicalContext()->MakeContextCurrent();

            if constexpr (HaveImguiPolicy) {
                ImguiPolicy::MakeImguiContextCurrent();
            }
        }
        
    private:
        virtual void UpdateFrame() final override {
            GraphicalContextPolicy::GetGraphicalContext()->MakeContextCurrent();

            FrameBase::OnUpdate();

            if constexpr (HaveImguiPolicy) {
                ImguiPolicy::MakeImguiContextCurrent();
            }

            if constexpr (HaveWindowPolicy) {
                WindowPolicy::GetWindow()->OnUpdate();
            }

            if constexpr (HaveImguiPolicy) {
                ImguiPolicy::GetImguiContext().UpdateViewPorts();
            }

            if constexpr (HavePanelPolicy) {
                PanelPolicy::UpdatePanels();
            }

            UpdateFrameUI();
        }

        void UpdateFrameUI() {
            RenderCommands::Clear();

            // updating imgui ui of the current frame and it's panels
            if constexpr (HaveImguiPolicy) {
                ImguiPolicy::GetImguiContext().Begin();
            }

            if constexpr (HaveWindowPolicy) {
                // resetting if the cursor is hovering over titlebar
                // thus it should be set by the titlebar in every frame 
                // titlebar should only set true in the requred condition
                WindowPolicy::GetWindow()->SetCanMoveWithCursor(false);
            }

            if constexpr (HaveImguiPolicy) {
                static_cast<ImguiPolicy*>(this)->OnImguiUiUpdate();
            }

            if constexpr (HaveTitleBarPolicy) {
                TitleBarPolicy::UpdateTitleBarUI();
            }

            if constexpr (HavePanelPolicy) {
                PanelPolicy::UpdatePanelsUI();
            }

            if constexpr (HaveImguiPolicy) {
                ImguiPolicy::GetImguiContext().End();
            }

            GraphicalContextPolicy::SwapBuffer();
        }

        virtual bool HandleEvent(Event& event) final override {
            FrameBase::OnEvent(event);

            if constexpr (HaveTitleBarPolicy) {
                TitleBarPolicy::TitleBarHandleEvents(event);
            }

            if constexpr (HavePanelPolicy) {
                PanelPolicy::HandlePanelsEvent(event);
            }

            return false;
        }
    };



    template<FrameFeature ...Features>
    requires ValidFrameFeatureSet<Features...>
    using FrameBaseType = FrameImpl<EnumValueListToTypeList_T<FramePolicyType, SortEnumValues_T<Features...>>>;

    template<FrameFeature ...Features>
    requires ValidFrameFeatureSet<FrameFeature::Window, FrameFeature::GraphicalContext, FrameFeature::ImGuiContext, Features...>
    using ImguiFrameBaseType = FrameImpl<EnumValueListToTypeList_T<
        FramePolicyType, 
        SortEnumValues_T<FrameFeature::Window, FrameFeature::GraphicalContext, FrameFeature::ImGuiContext, Features...>
    >>;

}


namespace Quirk {

    template<FrameFeature ...Features>
    struct Frame : public Internals::FrameBaseType<Features...> {
        Frame(const FrameInitContext& initContext) : 
                Internals::FrameBaseType<Features...>(initContext)
        {}
    };

    template<FrameFeature ...Features>
    struct ImguiFrame : public Internals::ImguiFrameBaseType<Features...> {
        ImguiFrame(const FrameInitContext& initContext) :
                Internals::ImguiFrameBaseType<Features...>(initContext)
        {}
    };

}
