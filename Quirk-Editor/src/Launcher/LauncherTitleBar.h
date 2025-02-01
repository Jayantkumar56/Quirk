
#pragma once

#include "Quirk.h"

namespace Quirk {

	class LauncherTitleBar : public TitleBar {
	public:
		LauncherTitleBar() {
			m_CloseIcon    = Texture2D::Create("assets/Images/close.png");
			m_MinimiseIcon = Texture2D::Create("assets/Images/minus.png");
			m_MaximiseIcon = Texture2D::Create("assets/Images/maximize.png");
		}

		~LauncherTitleBar() = default;

		virtual void OnImguiUiUpdate() override;
		virtual bool OnEvent(Event& event) override { return false; }

	private:
		Ref<Texture2D> m_CloseIcon;
		Ref<Texture2D> m_MinimiseIcon;
		Ref<Texture2D> m_MaximiseIcon;
	};

}
