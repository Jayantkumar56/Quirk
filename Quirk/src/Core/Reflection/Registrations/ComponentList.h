

#pragma once

#include "Core/Reflection/CommonMacros.h"

#include "Core/Utility/TypeTraits.h"
#include "Core/Scene/Components.h"

namespace Quirk {

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
        ( UUIDComponent,           "UUID"            ),
        ( TagComponent,            "Tag"             ),
        ( TransformComponent,      "Transform"       ),
        ( SpriteRendererComponent, "Sprite Renderer" ),
        ( CameraComponent,         "Camera"          ),
        ( NativeScriptComponent,   "Native Script"   ),
        ( MeshRendererComponent,   "Mesh"            ),
        ( LightComponent,          "Light"           )
    );

    // ============================================================================================================================



    // ============================================================================================================================
    //                                           ComponentTypeMeta Lists
    // ============================================================================================================================

    using ComponentTypesAll = TypeList<
        ComponentTypeMeta< UUIDComponent           >,
        ComponentTypeMeta< TagComponent            >,
        ComponentTypeMeta< TransformComponent      >,
        ComponentTypeMeta< SpriteRendererComponent >,
        ComponentTypeMeta< CameraComponent         >,
        ComponentTypeMeta< NativeScriptComponent   >,
        ComponentTypeMeta< MeshRendererComponent   >,
        ComponentTypeMeta< LightComponent          >
    >;

    using ComponentTypesIdentifiers = Quirk::TypeList<
        ComponentTypeMeta< UUIDComponent >,
        ComponentTypeMeta< TagComponent  >
    >;

    using ComponentTypesNonIdentifiers = Quirk::TypeList<
        ComponentTypeMeta< TransformComponent      >,
        ComponentTypeMeta< SpriteRendererComponent >,
        ComponentTypeMeta< CameraComponent         >,
        ComponentTypeMeta< NativeScriptComponent   >,
        ComponentTypeMeta< MeshRendererComponent   >,
        ComponentTypeMeta< LightComponent          >
    >;

    // ============================================================================================================================



    // ============================================================================================================================
    //                                                ComponentTypes Iterator
    // ============================================================================================================================

    template<typename TypeList, typename Func, typename ...Args>
    void ComponentsIterator(Func&& func, Args& ...args) {
        TypeList::ForEach(
            []<typename Pair, typename Func, typename ...Args>(Func& func, Args& ...args) {
                func.template operator() < typename Pair::Type > (Pair::Name, args...);
            },
            func,
            args...
        );
    }

    // ============================================================================================================================
}

