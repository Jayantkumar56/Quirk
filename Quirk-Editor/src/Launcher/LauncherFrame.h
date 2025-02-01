

#pragma once

#include "Quirk.h"
#include "FontManager.h"
#include "Launcher/LauncherTitleBar.h"

namespace Quirk {

	class LauncherFrame : public Frame {
	public:
		LauncherFrame(WindowSpecification& spec) :
			Frame(spec)
		{
			m_FontManager.LoadFonts();
			SetTitleBar<LauncherTitleBar>();
		}

		~LauncherFrame() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		virtual bool OnEvent(Event& event) override;
		virtual void OnUpdate()			   override;
		virtual void OnImguiUiUpdate()     override;

		inline FontManager& GetFontManager() { return m_FontManager; }

	private:
		FontManager m_FontManager;
	};

}
