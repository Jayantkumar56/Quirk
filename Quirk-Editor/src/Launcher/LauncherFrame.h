

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

			LoadFonts();
			SetColorTheme();

			SetTitleBar<LauncherTitleBar>();

			m_ProjectIcon		= Texture2D::Create("assets/Images/Launcher/project.png");
			m_OpenProjectIcon   = Texture2D::Create("assets/Images/Launcher/openFolder.png");
			m_CreateProjectIcon = Texture2D::Create("assets/Images/Launcher/createProject.png");

			Project::LoadRecentProjectsList("RecentProjects.yaml");
		}

		virtual bool OnEvent(Event& event) override { return false; }
		virtual void OnUpdate()			   override {}
		virtual void OnImguiUiUpdate()     override;

		inline FontManager& GetFontManager() { return m_FontManager; }

	private:
		void LoadFonts();
		void SetColorTheme();

	private:
		FontManager    m_FontManager;
		Ref<Texture2D> m_ProjectIcon;
		Ref<Texture2D> m_OpenProjectIcon;
		Ref<Texture2D> m_CreateProjectIcon;
	};

}
