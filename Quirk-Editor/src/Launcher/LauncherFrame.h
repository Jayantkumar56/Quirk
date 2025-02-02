

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
			// removing imgui window borders especially for titlebar
			ImGuiStyle& style = ImGui::GetStyle();
			style.WindowBorderSize = 0.0f;

			SetColorTheme();
			m_FontManager.LoadFonts();
			m_FontManager.LoadFont(ImGui::GetIO(), FontWeight::Regular, 50);
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
		void SetColorTheme();

	private:
		FontManager m_FontManager;
	};

}
