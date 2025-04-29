

#pragma once

#include "Quirk.h"
#include "Launcher/LauncherTitleBar.h"

#include "Base/ProjectManager.h"

namespace QuirkEditor {

	class LauncherFrame : public Quirk::Frame<Quirk::PanelPolicy::Disabled, Quirk::TitleBarPolicy::Enabled> {
	private:
		// two different states for two different ui
		enum class LauncherState {
			MainMenu,					// for the initial main menu 
			ProjectForm					// for new project creation form
		};

	public:
		LauncherFrame(Quirk::WindowSpecification& spec) :
			Frame(spec)
		{
			// initailly MainMenu will be loaded
			m_State = LauncherState::MainMenu;
			SetColorTheme();

			SetTitleBar<LauncherTitleBar>(this);

			m_ProjectIcon		= Quirk::Texture2D::Create("assets/Images/Launcher/project.png");
			m_OpenProjectIcon   = Quirk::Texture2D::Create("assets/Images/Launcher/openFolder.png");
			m_CreateProjectIcon = Quirk::Texture2D::Create("assets/Images/Launcher/createProject.png");

            Quirk::ProjectManager::LoadRecentProjectsList("RecentProjects.yaml");

			// reserving some storage to get input through imgui
			m_TempProject.Title = "Untitled";
			m_TempProject.Title.resize(32);
			m_TempProjPath.resize(512);
		}

		virtual ~LauncherFrame() {
            Quirk::ProjectManager::SaveRecentProjectsList("RecentProjects.yaml");
		}

		virtual bool OnEvent(Quirk::Event& event) override { return false; }
		virtual void OnUpdate()			   override {}
		virtual void OnImguiUiUpdate()     override;

	private:
		void DrawMainMenu();
		void DrawProjectCreationForm();
		void SetColorTheme();

	private:
		Quirk::Ref<Quirk::Texture2D> m_ProjectIcon;
		Quirk::Ref<Quirk::Texture2D> m_OpenProjectIcon;
		Quirk::Ref<Quirk::Texture2D> m_CreateProjectIcon;
		LauncherState  m_State;

		// used for temporary things (in the creation of new project)
		// some storage should be reserved to the member strings even when the strings are empty 
		// so that it could be used for input from imgui
        Quirk::ProjectMetadata m_TempProject;
		std::string     m_TempProjPath;
	};

}
