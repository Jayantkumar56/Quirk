

#pragma once

#include "Quirk.h"

namespace Quirk {

	class SceneHierarchyPanel {
	public:
		SceneHierarchyPanel() = default;
		~SceneHierarchyPanel() = default;

		void OnImguiUiUpdate(const Ref<Scene>& scene, Entity& selectedEntity);

	private:
		void DrawEntityNode(Entity entity, Entity& selectedEntity);

	private:
		bool m_PopupOpened = false;
	};

}
