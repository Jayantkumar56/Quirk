

#pragma once


#include "WindowManager.h"
#include "GraphicalContextManager.h"
#include "ImguiContextManager.h"
#include "PanelManager.h"
#include "TitleBarManager.h"
#include "Core/Utility/TypeTraits.h"


namespace Quirk {

    // NOTE: 
    //
    // - the value of these enums decides
    //   the order of inheritance of respective policy in the Frame
    //   (i.e. the feature policies are sorted on the basis of these enum values)

    enum class FrameFeature {
        Window            = 0,
        GraphicalContext  = 1,
        ImGuiContext      = 2,
        TitleBar          = 3,
        Panels            = 4
    };

}


namespace Quirk::Internals {

    template<FrameFeature Feature>
    struct FramePolicyType {
        static_assert(AlwaysFalse_V<Feature>, "FramePolicyType not specialized for this policy value.");
    };

    template<> struct FramePolicyType<FrameFeature::Window>           { using Type = WindowManager;           };
    template<> struct FramePolicyType<FrameFeature::GraphicalContext> { using Type = GraphicalContextManager; };
    template<> struct FramePolicyType<FrameFeature::ImGuiContext>     { using Type = ImguiContextManager;     };
    template<> struct FramePolicyType<FrameFeature::Panels>           { using Type = PanelManager;            };
    template<> struct FramePolicyType<FrameFeature::TitleBar>         { using Type = TitleBarManager;         };

    template<FrameFeature Feature>
    using FramePolicyType_T = FramePolicyType<Feature>::Type;



    template<FrameFeature ...Features>
    concept ValidImguiFeature = (
        IsEnumValuePresent_V<FrameFeature::Window,       Features...>
    ) || (
        !IsEnumValuePresent_V<FrameFeature::Window,       Features...> &&
        !IsEnumValuePresent_V<FrameFeature::ImGuiContext, Features...>
    );

    template<FrameFeature ...Features>
    concept ValidPanelFeature = (
        IsEnumValuePresent_V<FrameFeature::ImGuiContext, Features...>
    ) || (
        !IsEnumValuePresent_V<FrameFeature::Panels,       Features...> &&
        !IsEnumValuePresent_V<FrameFeature::ImGuiContext, Features...>
    );

    template<FrameFeature ...Features>
    concept ValidTitleBarFeature = (
        IsEnumValuePresent_V<FrameFeature::ImGuiContext, Features...>
    ) || (
        !IsEnumValuePresent_V<FrameFeature::TitleBar,     Features...> &&
        !IsEnumValuePresent_V<FrameFeature::ImGuiContext, Features...>
    );

    template<FrameFeature ...Features>
    concept ValidFrameFeatureSet = NoDuplicatesInEnumValues <Features...> &&
                                   ValidImguiFeature        <Features...> &&
                                   ValidPanelFeature        <Features...> &&
                                   ValidTitleBarFeature     <Features...>;

}
