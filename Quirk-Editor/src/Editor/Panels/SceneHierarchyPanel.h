

#pragma once

#include "Quirk.h"

namespace QuirkEditor {

	class SceneHierarchyPanel : public Quirk::Panel {
	public:
		SceneHierarchyPanel() : Quirk::Panel("Scene Hierarchy") { }
		~SceneHierarchyPanel() = default;

		virtual void SetUiProperties() override;
		virtual void OnUiUpdate()    override;

	private:
		void DrawEntityNode(Quirk::Entity entity, Quirk::Entity& selectedEntity);
	};

}
