

#pragma once


#include "InspectorPanel.h"
#include "Editor/EditorFrame.h"

#include "Core/Scene/Components.h"
#include "Core/Utility/TypeTraits.h"
#include "Core/Utility/View.h"

namespace QuirkEditor {
    
    template<>
    class InspectorDraw<Quirk::Entity> {
    public:
        static void Draw(Quirk::View<EditorFrame> frame, Quirk::Entity);

    private:
        template<typename T>
        static void ComponentDraw(Quirk::View<EditorFrame> frame, T& component) {
            static_assert(Quirk::AlwaysFalse_V<T>, "Draw for the component is not supported yet!");
        }

        // Specializations for each component to draw

        template<> static void ComponentDraw( Quirk::View<EditorFrame> frame, Quirk::UUIDComponent&           );
        template<> static void ComponentDraw( Quirk::View<EditorFrame> frame, Quirk::TagComponent&            );
        template<> static void ComponentDraw( Quirk::View<EditorFrame> frame, Quirk::TransformComponent&      );
        template<> static void ComponentDraw( Quirk::View<EditorFrame> frame, Quirk::SpriteRendererComponent& );
        template<> static void ComponentDraw( Quirk::View<EditorFrame> frame, Quirk::CameraComponent&         );
        template<> static void ComponentDraw( Quirk::View<EditorFrame> frame, Quirk::NativeScriptComponent&   );
        template<> static void ComponentDraw( Quirk::View<EditorFrame> frame, Quirk::MeshRendererComponent&   );
        template<> static void ComponentDraw( Quirk::View<EditorFrame> frame, Quirk::LightComponent&          );
    };

}

