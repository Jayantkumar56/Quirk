
#pragma once

#include "Quirk.h"

namespace Quirk {

	class EditorTitleBar : public TitleBar{
	public:
		EditorTitleBar() = default;
		~EditorTitleBar() = default;

		virtual void OnImguiUiUpdate() override;
		virtual bool OnEvent(Event& event) override { return false; }
	};

}
