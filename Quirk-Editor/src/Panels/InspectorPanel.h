

#pragma once

#include "Quirk.h"

namespace Quirk {

	class InspectorPanel {
	public:
		InspectorPanel() = default;
		~InspectorPanel() = default;

		void OnImguiUiUpdate(Entity& entity);
		
	private:
		static bool DrawFloat3(const char* label, float* data, float resetValue = 0.0f, float labelColumnWidth = 100.0f, float speed = 0.1f);
	};

}
