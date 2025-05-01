

#pragma once

#include "Base/Reflection/CommonMacros.h"

#include "Core/Utility/TypeTraits.h"
#include "Core/Scene/Components.h"

namespace QuirkEditor {

    // ============================================================================================================================
    //                                           ComponentTypeMeta Registration
    // ============================================================================================================================

    template<typename T>
    struct ComponentTypeMeta {
        using Type = T;
        static constexpr std::string_view Name = "Unknown";
    };

#define REGISTER_COMPONENT_META_HELPER(t)                                 \
    template<>                                                            \
    struct ComponentTypeMeta< TUPLE_GET_FIRST(t) > {                      \
        using Type = TUPLE_GET_FIRST(t);                                  \
        static constexpr std::string_view Name = TUPLE_GET_SECOND(t);     \
    };

#define REGISTER_COMPONENT_META(...)                                      \
    FOR_EACH(REGISTER_COMPONENT_META_HELPER, __VA_ARGS__)


    REGISTER_COMPONENT_META(
        ( Quirk::UUIDComponent,           "UUID"            ),
        ( Quirk::TagComponent,            "Tag"             ),
        ( Quirk::TransformComponent,      "Transform"       ),
        ( Quirk::SpriteRendererComponent, "Sprite Renderer" ),
        ( Quirk::CameraComponent,         "Camera"          ),
        ( Quirk::NativeScriptComponent,   "Native Script"   ),
        ( Quirk::MeshRendererComponent,   "Mesh"            ),
        ( Quirk::LightComponent,          "Light"           )
    );

    // ============================================================================================================================



    // ============================================================================================================================
    //                                           ComponentTypeMeta Lists
    // ============================================================================================================================

    using ComponentTypesAll = Quirk::TypeList<
        ComponentTypeMeta< Quirk::UUIDComponent           >,
        ComponentTypeMeta< Quirk::TagComponent            >,
        ComponentTypeMeta< Quirk::TransformComponent      >,
        ComponentTypeMeta< Quirk::SpriteRendererComponent >,
        ComponentTypeMeta< Quirk::CameraComponent         >,
        ComponentTypeMeta< Quirk::NativeScriptComponent   >,
        ComponentTypeMeta< Quirk::MeshRendererComponent   >,
        ComponentTypeMeta< Quirk::LightComponent          >
    >;

    using ComponentTypesIdentifiers = Quirk::TypeList<
        ComponentTypeMeta< Quirk::UUIDComponent >,
        ComponentTypeMeta< Quirk::TagComponent  >
    >;

    using ComponentTypesNonIdentifiers = Quirk::TypeList<
        ComponentTypeMeta< Quirk::TransformComponent      >,
        ComponentTypeMeta< Quirk::SpriteRendererComponent >,
        ComponentTypeMeta< Quirk::CameraComponent         >,
        ComponentTypeMeta< Quirk::NativeScriptComponent   >,
        ComponentTypeMeta< Quirk::MeshRendererComponent   >,
        ComponentTypeMeta< Quirk::LightComponent          >
    >;

    // ============================================================================================================================



    // ============================================================================================================================
    //                                                ComponentTypes Iterator
    // ============================================================================================================================

    template<typename TypeList, typename Func, typename ...Args>
    void ComponentsIterator(Func&& func, Args&& ...args) {
        TypeList::ForEach(
            [&]<typename Pair, typename Func, typename ...Args>(Func && func, Args&& ...args) {
            func.template operator() < typename Pair::Type > (Pair::Name, std::forward<Args>(args)...);
        },
            std::forward<Func>(func),
            std::forward<Args>(args)...
        );
    }

    // ============================================================================================================================
}

