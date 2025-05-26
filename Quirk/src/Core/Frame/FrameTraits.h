

#pragma once


#include "Window.h"
#include "TitleBar.h"
#include "Panel.h"
#include "GraphicalContext.h"


namespace Quirk {

    // Policies
    enum class PanelPolicy    { Disabled, Enabled };
    enum class TitleBarPolicy { Disabled, Enabled };

    // Empty fallback types
    struct NoPanel    {};
    struct NoTitleBar {};


    template<auto V> 
    requires std::is_enum_v<decltype(V)>
    struct FramePolicyToType {
        static constexpr bool AlwaysFalse = false;
        static_assert(AlwaysFalse, "EnumPolicyToType not specialized for this policy value.");
    };

    template<> struct FramePolicyToType<PanelPolicy::Enabled>  { using type = PanelManager; };
    template<> struct FramePolicyToType<PanelPolicy::Disabled> { using type = NoPanel;      };

    template<> struct FramePolicyToType<TitleBarPolicy::Enabled>  { using type = TitleBarManager; };
    template<> struct FramePolicyToType<TitleBarPolicy::Disabled> { using type = NoTitleBar;      };

}

