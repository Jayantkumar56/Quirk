

#pragma once

#include "Base/SelectionContext.h"

#include "Core/Frame/Panel.h"
#include "Core/Scene/Entity.h"
#include "Core/Utility/TypeTraits.h"


namespace QuirkEditor {

    template<typename T>
    class InspectorDraw {
        // Note: general draw is not yet supported 
        // (must define a specialization for the type to be drawn)
        static_assert(Quirk::AlwaysFalse_V<T>, "Draw is not defined for provided type!");

        // Note: 
        // 
        // - function with following name and signature must exist in specialization:-
        // 
        //   pulbic: static void Draw(...) 
        // 
        //   This function will be called to draw the UI for the given type.
    };

    class InspectorPanel : public Quirk::Panel {
    public:
        InspectorPanel() :
                Quirk::Panel( "Inspector"     )
        {}

        ~InspectorPanel() = default;

        virtual void SetUiProperties() override;
        virtual void OnUiUpdate()      override;

    private:
        UniqueSelectionHandle<Quirk::Entity> m_SelectionHandle;
    };

}

