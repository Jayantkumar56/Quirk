

#pragma once

#include "Quirk.h"

namespace Quirk {

	class InspectorPanel {
	public:
		InspectorPanel() = default;
		~InspectorPanel() = default;

		void OnImguiUiUpdate(Entity& entity);
	};

}
