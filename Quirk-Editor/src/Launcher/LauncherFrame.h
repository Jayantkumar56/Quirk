

#pragma once

#include "Quirk.h"
#include "FontManager.h"
#include "Launcher/LauncherTitleBar.h"

namespace Quirk {

	class LauncherFrame : public Frame {
	private:
		// two different states for two different ui
		enum class LauncherState {
			MainMenu,					// for the initial main menu 
			ProjectForm					// for new project creation form
		};

	public:
		LauncherFrame(WindowSpecification& spec) :
			Frame(spec)
		{
			// initailly MainMenu will be loaded
			m_State = LauncherState::MainMenu;

			LoadFonts();
			SetColorTheme();

			SetTitleBar<LauncherTitleBar>();

			m_ProjectIcon		= Texture2D::Create("assets/Images/Launcher/project.png");
			m_OpenProjectIcon   = Texture2D::Create("assets/Images/Launcher/openFolder.png");
			m_CreateProjectIcon = Texture2D::Create("assets/Images/Launcher/createProject.png");

			Project::LoadRecentProjectsList("RecentProjects.yaml");

			// reserving some storage to get input through imgui
			m_TempProject.Title = "Untitled";
			m_TempProject.Title.resize(32);
			m_TempProjPath.resize(512);
		}

		virtual ~LauncherFrame() {
			Project::SaveRecentProjectsList("RecentProjects.yaml");
		}

		virtual bool OnEvent(Event& event) override { return false; }
		virtual void OnUpdate()			   override {}
		virtual void OnImguiUiUpdate()     override;

		inline FontManager& GetFontManager() { return m_FontManager; }

	private:
		void DrawMainMenu();
		void DrawProjectCreationForm();

		void LoadFonts();
		void SetColorTheme();

	private:
		FontManager    m_FontManager;
		Ref<Texture2D> m_ProjectIcon;
		Ref<Texture2D> m_OpenProjectIcon;
		Ref<Texture2D> m_CreateProjectIcon;
		LauncherState  m_State;

		// used for temporary things (in the creation of new project)
		// some storage should be reserved to the member strings even when the strings are empty 
		// so that it could be used for input from imgui
		ProjectMetadata m_TempProject;
		std::string     m_TempProjPath;
	};

}
