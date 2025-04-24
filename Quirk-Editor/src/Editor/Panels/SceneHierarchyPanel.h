

#pragma once

#include "Quirk.h"

namespace QuirkEditor {

	class SceneHierarchyPanel : public Quirk::Panel {
	public:
		SceneHierarchyPanel() : Quirk::Panel("Scene Hierarchy") { }
		~SceneHierarchyPanel() = default;

		virtual void SetImguiProperties() override;
		virtual void OnImguiUiUpdate()    override;

	private:
		void DrawEntityNode(Quirk::Entity entity, Quirk::Entity& selectedEntity);
	};

}
