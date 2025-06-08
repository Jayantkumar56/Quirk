

#pragma once

#include "Base/ProjectManager.h"
#include "Launcher/LauncherTitleBar.h"

#include "Core/Frame/Frame.h"
#include "Core/AssetManager/AssetImporter/TextureImporter.h"


namespace QuirkEditor {

	class LauncherFrame final : public Quirk::ImguiFrame<Quirk::FrameFeature::TitleBar> {
	private:
		// two different states for two different ui
		enum class LauncherState {
			MainMenu,					// for the initial main menu 
			ProjectForm					// for new project creation form
		};

	public:
		LauncherFrame(ProjectManager& projManager) noexcept :
                ImguiFrame       ( GetFrameSpec() ),
                m_ProjectManager ( projManager    )
		{}

        virtual void Init()      noexcept override;
        virtual void Terminate() noexcept override;

		virtual void OnImguiUiUpdate() override;

	private:
		void DrawMainMenu();
		void DrawProjectCreationForm();
		void SetColorTheme();

        inline Quirk::FrameInitContext GetFrameSpec() const noexcept {
            return Quirk::FrameInitContext{
                .WindowSpec{
                    .Title             { "Quirk Engine" },
                    .Width             { 1100           },      .Height    { 780   },
                    .MinWidth          { 1100           },      .MinHeight { 780   },
                    .PosX              { 200            },      .PosY      { 50    },
                    .VSyncOn           { true           },      .Maximized { false },
                    .CustomTitleBar    { true           }
                },
                .GraphicsAPI{
                    Quirk::RHI::GraphicsAPI::OpenGL
                }
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
