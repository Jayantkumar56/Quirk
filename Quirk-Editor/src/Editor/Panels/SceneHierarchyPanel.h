

#pragma once

#include "Quirk.h"
#include "Base/SelectionContext.h"


namespace QuirkEditor {

	class SceneHierarchyPanel : public Quirk::Panel {
	public:
		SceneHierarchyPanel() : Quirk::Panel("Scene Hierarchy") { }
		~SceneHierarchyPanel() = default;

		virtual void SetUiProperties() override;
		virtual void OnUiUpdate()    override;

	private:
		void DrawEntityNode(Quirk::Entity entity);

    private:
        PerTypeSelectionHandle<Quirk::Entity> m_SelectionHandle;
	};

}
