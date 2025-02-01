

#pragma once

#include "Quirk.h"

namespace Quirk {

	class SceneHierarchyPanel : public Panel {
	public:
		SceneHierarchyPanel() = default;
		~SceneHierarchyPanel() = default;

		virtual void OnUpdate() override { }
		virtual bool OnEvent(Event& event) override { return false; }
		virtual void OnImguiUiUpdate() override;

	private:
		void DrawEntityNode(Entity entity, Entity& selectedEntity);
	};

}
