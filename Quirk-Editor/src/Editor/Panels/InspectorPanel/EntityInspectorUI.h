

#pragma once


#include "InspectorPanel.h"

#include "Core/Scene/Components.h"
#include "Core/Utility/TypeTraits.h"


namespace QuirkEditor {
    
    template<>
    class InspectorDraw<Quirk::Entity> {
    public:
        static void Draw(Quirk::Entity);

    private:
        template<typename T>
        static void ComponentDraw(T& component) {
            static_assert(Quirk::AlwaysFalseV<T>, "Draw for the component is not supported yet!");
        }

        // Specializations for each component to draw

        template<> static void ComponentDraw( Quirk::UUIDComponent&           );
        template<> static void ComponentDraw( Quirk::TagComponent&            );
        template<> static void ComponentDraw( Quirk::TransformComponent&      );
        template<> static void ComponentDraw( Quirk::SpriteRendererComponent& );
        template<> static void ComponentDraw( Quirk::CameraComponent&         );
        template<> static void ComponentDraw( Quirk::NativeScriptComponent&   );
        template<> static void ComponentDraw( Quirk::MeshRendererComponent&   );
        template<> static void ComponentDraw( Quirk::LightComponent&          );
    };

}

