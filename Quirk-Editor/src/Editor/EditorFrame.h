

#pragma once


#include "EditorTitleBar.h"
#include "Panels/SceneViewportPanel.h"
#include "Panels/SceneHierarchyPanel.h"
#include "Panels/InspectorPanel/InspectorPanel.h"
#include "Panels/ContentBrowserPanel.h"
#include "EditorTheme.h"
#include "EditorFrameResourceManager.h"

#include "Core/Frame/Frame.h"

namespace QuirkEditor {

	class EditorFrame : 
        public Quirk::Frame<
            Quirk::PanelPolicy::Enabled, 
            Quirk::TitleBarPolicy::Enabled
        >
    {
	public:
		EditorFrame(ProjectManager& projManager) :
				Frame     ( GetEditorFrameWindowSpec() ),
                m_Project ( projManager.GetActive()    )
		{

			Quirk::Renderer::InitRenderer();
			Quirk::Renderer2D::InitRenderer();

            m_Theme.SetTheme(ThemeName::DarkTheme);

			SetTitleBar<EditorTitleBar>(this);

			AddPanel<SceneViewportPanel> (this);
			AddPanel<SceneHierarchyPanel>(this);
			AddPanel<InspectorPanel>     (this);
			AddPanel<ContentBrowserPanel>(this, m_Project->GetAssetDirectory());
		}

		virtual void OnImguiUiUpdate() override {
			// Disabling alt key for imgui to prevent navigation with alt key (problems when using editor cotrols)
			ImGui::SetKeyOwner(ImGuiKey_LeftAlt, ImGuiKeyOwner_Any, ImGuiInputFlags_LockThisFrame);
		}

        inline auto& GetProjectRefView()     noexcept { return m_Project;                          }
        inline auto& GetActiveSceneRefView() noexcept { return m_Project->GetActiveSceneRefView(); }

        inline EditorTheme& GetTheme() noexcept { return m_Theme; }
        inline EditorFrameResourceManager& GetResourceManager() noexcept { return m_ResourceManager; }

    private:
        inline Quirk::WindowSpecification GetEditorFrameWindowSpec() {
            return Quirk::WindowSpecification{
			    .Title             { "Quirk Editor" },
			    .Width             { 1600           },		.Height    { 900  },
			    .MinWidth          { 1600           },		.MinHeight { 900  },
			    .PosX              { 200            },		.PosY      { 50   },
			    .VSyncOn           { true           },		.Maximized { true },
			    .CustomTitleBar    { true           }
		    };
        }

	private:
        Quirk::Ref<Quirk::Project> m_Project;

        EditorTheme                m_Theme;
        EditorFrameResourceManager m_ResourceManager;
	};

}
