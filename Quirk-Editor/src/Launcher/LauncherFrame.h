

#pragma once

#include "Base/ProjectManager.h"
#include "Launcher/LauncherTitleBar.h"

#include "Core/Frame/Frame.h"
#include "Core/AssetManager/AssetImporter/TextureImporter.h"


namespace QuirkEditor {

	class LauncherFrame : 
        public Quirk::Frame <
            Quirk::PanelPolicy::Disabled, 
            Quirk::TitleBarPolicy::Enabled
        > 
    {
	private:
		// two different states for two different ui
		enum class LauncherState {
			MainMenu,					// for the initial main menu 
			ProjectForm					// for new project creation form
		};

	public:
		LauncherFrame(ProjectManager& projManager) :
			Frame            ( GetEditorFrameWindowSpec() ),
            m_ProjectManager ( projManager                )
		{
			// initailly MainMenu will be loaded
			m_State = LauncherState::MainMenu;
			SetColorTheme();

			SetTitleBar<LauncherTitleBar>(this);

			m_ProjectIcon		= Quirk::TextureImporter::CreateFromImage( "assets/Images/Launcher/project.png"       );
			m_OpenProjectIcon   = Quirk::TextureImporter::CreateFromImage( "assets/Images/Launcher/openFolder.png"    );
			m_CreateProjectIcon = Quirk::TextureImporter::CreateFromImage( "assets/Images/Launcher/createProject.png" );


			// reserving some storage to get input through imgui
			m_TempProject.Title = "Untitled";
			m_TempProject.Title.resize(32);
			m_TempProjPath.resize(512);
		}

		virtual void OnImguiUiUpdate() override;

	private:
		void DrawMainMenu();
		void DrawProjectCreationForm();
		void SetColorTheme();

        inline Quirk::WindowSpecification GetEditorFrameWindowSpec() const noexcept {
            return Quirk::WindowSpecification{
                .Title             { "Quirk Engine" },
			    .Width             { 1100           },      .Height    { 780   },
			    .MinWidth          { 1100           },      .MinHeight { 780   },
			    .PosX              { 200            },      .PosY      { 50    },
			    .VSyncOn           { true           },      .Maximized { false },
			    .CustomTitleBar    { true           }
            };
        }

	private:
		Quirk::Ref<Quirk::Texture2D> m_ProjectIcon;
		Quirk::Ref<Quirk::Texture2D> m_OpenProjectIcon;
		Quirk::Ref<Quirk::Texture2D> m_CreateProjectIcon;
		LauncherState  m_State;

		// used for temporary things (in the creation of new project)
		// some storage should be reserved to the member strings even when the strings are empty 
		// so that it could be used for input from imgui
        ProjectMetadata m_TempProject;
		std::string     m_TempProjPath;
        ProjectManager& m_ProjectManager;
	};

}
