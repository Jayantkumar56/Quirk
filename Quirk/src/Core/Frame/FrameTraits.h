

#pragma once


#include "WindowManager.h"
#include "GraphicalContextManager.h"
#include "ImguiContextManager.h"
#include "PanelManager.h"
#include "TitleBarManager.h"
#include "Core/Utility/TypeTraits.h"


namespace Quirk {

    enum class FrameFeature {
        Window,
        ImGuiContext,
        TitleBar,
        Panels
    };



    template<FrameFeature ...Features>
    constexpr bool ValidImguiFeature_V = (
        IsEnumValuePresent_V<FrameFeature::Window,       Features...> &&
        IsEnumValuePresent_V<FrameFeature::ImGuiContext, Features...>
    ) || (
        !IsEnumValuePresent_V<FrameFeature::Window,       Features...> &&
        !IsEnumValuePresent_V<FrameFeature::ImGuiContext, Features...>
    );

    template<FrameFeature ...Features>
    constexpr bool ValidPanelFeature_V = (
        IsEnumValuePresent_V<FrameFeature::Panels,       Features...> &&
        IsEnumValuePresent_V<FrameFeature::ImGuiContext, Features...>
    ) || (
        !IsEnumValuePresent_V<FrameFeature::Panels,       Features...> &&
        !IsEnumValuePresent_V<FrameFeature::ImGuiContext, Features...>
    );

    template<FrameFeature ...Features>
    constexpr bool ValidTitleBarFeature_V = (
        IsEnumValuePresent_V<FrameFeature::TitleBar,     Features...> &&
        IsEnumValuePresent_V<FrameFeature::ImGuiContext, Features...>
    ) || (
        !IsEnumValuePresent_V<FrameFeature::TitleBar,     Features...> &&
        !IsEnumValuePresent_V<FrameFeature::ImGuiContext, Features...>
    );

    template<FrameFeature ...Features>
    concept ValidFrameFeatureSet = !DuplicatesInEnumValues_V <Features...> &&
                                   ValidImguiFeature_V       <Features...> &&
                                   ValidPanelFeature_V       <Features...> &&
                                   ValidTitleBarFeature_V    <Features...>;



    template<FrameFeature Feature>
    struct FramePolicyType {
        static_assert(AlwaysFalse_V<Feature>, "EnumPolicyToType not specialized for this policy value.");
    };

    template<> struct FramePolicyType<FrameFeature::Window>       { using Type = WindowManager;       };
    template<> struct FramePolicyType<FrameFeature::ImGuiContext> { using Type = ImguiContextManager; };
    template<> struct FramePolicyType<FrameFeature::Panels>       { using Type = PanelManager;        };
    template<> struct FramePolicyType<FrameFeature::TitleBar>     { using Type = TitleBarManager;     };

    template<FrameFeature Feature>
    using FramePolicyType_T = FramePolicyType<Feature>::Type;



    template<FrameFeature Feature, typename FeatureList, FrameFeature... Features>
    struct InsertIfContainFeature;

    template<FrameFeature Feature, typename... Ts, FrameFeature... Features>
    struct InsertIfContainFeature<Feature, TypeList<Ts...>, Features...> {
        using Type = std::conditional_t<
            IsEnumValuePresent_V<Feature, Features...>,
            TypeList<FramePolicyType_T<Feature>, Ts...>,
            TypeList<Ts...>
        >;
    };

    template<FrameFeature Feature, typename FeatureList, FrameFeature ...Features>
    using InsertIfContainFeature_T = InsertIfContainFeature<Feature, FeatureList, Features...>::Type;



    template<FrameFeature ...Features>
    struct SortedFramePolicies {
        using Type = InsertIfContainFeature_T<
            FrameFeature::Window,
            PushFront_T<
                InsertIfContainFeature_T<
                    FrameFeature::ImGuiContext,
                    InsertIfContainFeature_T<
                        FrameFeature::Panels,
                        InsertIfContainFeature_T<
                            FrameFeature::TitleBar, TypeList<>, Features...
                        >,
                        Features...
                    >,
                    Features...
                >,
                GraphicalContextManager
            >,
            Features...
        >;
    };

    template<FrameFeature ...Features>
    using SortedFramePolicies_T = SortedFramePolicies<Features...>::Type;



    template<typename FramePolicy, typename FrameT>
    struct PolicyConstructorArgs;

    template<typename FrameT>
    struct PolicyConstructorArgs<FramePolicyType_T<FrameFeature::Window>, FrameT> {
        using Tuple = std::tuple<const WindowSpecification&>;
        static Tuple Get(FrameT& frame, const WindowSpecification& spec) {
            return Tuple(spec);
        }
    };

    template<typename FrameT>
    struct PolicyConstructorArgs<GraphicalContextManager, FrameT> {
        using Tuple = std::tuple<Window&>;
        static Tuple Get(FrameT& frame, const WindowSpecification& spec) {
            return Tuple(frame.GetWindow());
        }
    };

    template<typename FrameT>
    struct PolicyConstructorArgs<FramePolicyType_T<FrameFeature::ImGuiContext>, FrameT> {
        using Tuple = std::tuple<FrameT&>;
        static Tuple Get(FrameT& frame, const WindowSpecification&) {
            return Tuple(frame);
        }
    };

    template<typename FrameT>
    struct PolicyConstructorArgs<FramePolicyType_T<FrameFeature::Panels>, FrameT> {
        using Tuple = std::tuple<>;
        static Tuple Get(FrameT&, const WindowSpecification&) {
            return Tuple();
        }
    };

    template<typename FrameT>
    struct PolicyConstructorArgs<FramePolicyType_T<FrameFeature::TitleBar>, FrameT> {
        using Tuple = std::tuple<>;
        static Tuple Get(FrameT&, const WindowSpecification&) {
            return Tuple();
        }
    };



    template<typename FrameT>
    struct FramePolicyTupleGetterAdapter {
        template<typename Policy>
        using Getter = PolicyConstructorArgs<Policy, FrameT>;
    };

}

