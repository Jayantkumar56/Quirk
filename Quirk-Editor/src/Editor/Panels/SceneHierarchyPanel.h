

#pragma once

#include "Editor/EditorFrame.h"
#include "Base/SelectionContext.h"

#include "Core/Frame/Panel.h"
#include "Core/Scene/Entity.h"

namespace QuirkEditor {

	class SceneHierarchyPanel : public Quirk::Panel {
	public:
		SceneHierarchyPanel(EditorFrame* frame) : 
                Panel(frame, "Scene Hierarchy")
        {}

		virtual void SetUiProperties() override;
		virtual void OnUiUpdate()    override;

	private:
		void DrawEntityNode(Quirk::Entity entity);

    private:
        PerTypeSelectionHandle<Quirk::Entity> m_SelectionHandle;
	};

}
