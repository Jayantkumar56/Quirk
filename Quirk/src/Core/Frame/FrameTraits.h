

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
    struct EnumPolicyToType {
        static constexpr bool AlwaysFalse = false;
        static_assert(AlwaysFalse, "EnumPolicyToType not specialized for this policy value.");
    };

    template<> struct EnumPolicyToType<PanelPolicy::Enabled>  { using Type = PanelManager; };
    template<> struct EnumPolicyToType<PanelPolicy::Disabled> { using Type = NoPanel; };

    template<> struct EnumPolicyToType<TitleBarPolicy::Enabled>  { using Type = TitleBarManager;   };
    template<> struct EnumPolicyToType<TitleBarPolicy::Disabled> { using Type = NoTitleBar; };

}

