

#pragma once

#include "Quirk.h"

namespace Quirk {

	class SceneHierarchyPanel : public Panel {
	public:
		SceneHierarchyPanel() : Panel("Scene Hierarchy") { }
		~SceneHierarchyPanel() = default;

		virtual void OnImguiUiUpdate()      override;

	private:
		void DrawEntityNode(Entity entity, Entity& selectedEntity);
	};

}
