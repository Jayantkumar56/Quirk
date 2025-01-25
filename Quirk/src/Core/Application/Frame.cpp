
#include "Qkpch.h"
#include "Frame.h"


namespace Quirk {

	Window& Quirk::TitleBar::GetWindow() {
		return m_ParentFrame->GetWindow();
	}

	Window& Panel::GetWindow() {
		return m_ParentFrame->GetWindow();
	}

}

