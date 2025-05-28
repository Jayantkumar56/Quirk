

#pragma once


#include "EditorTheme.h"
#include "EditorFrameResourceManager.h"
#include "Base/ProjectManager.h"

#include "Core/Frame/Frame.h"


namespace QuirkEditor {

    enum class EditorMode {
        Edit,
        Play
    };

	class EditorFrame : 
        public Quirk::Frame<
            Quirk::PanelPolicy::Enabled,
            Quirk::TitleBarPolicy::Enabled
        >
    {
	public:
		EditorFrame(Quirk::Scope<Quirk::Project> project) noexcept :
				Frame        ( GetEditorFrameWindowSpec() ),
                m_EditorMode ( EditorMode::Edit           ),
                m_Project    ( std::move(project)         )
		{}

        void Init() noexcept;

		virtual void OnImguiUiUpdate() override {
			// Disabling alt key for imgui to prevent navigation with alt key (problems when using editor cotrols)
			ImGui::SetKeyOwner(ImGuiKey_LeftAlt, ImGuiKeyOwner_Any, ImGuiInputFlags_LockThisFrame);
		}

        inline Quirk::View<Quirk::Scene> GetActiveSceneView() noexcept { return m_Project->GetActiveSceneRefView(); }
        inline Quirk::Ref<Quirk::Scene>  GetActiveScene()     noexcept { return m_Project->GetActiveSceneRefView(); }

        inline const EditorMode GetEditorMode()    const noexcept { return m_EditorMode; }
        inline void SetEditorMode(EditorMode mode)       noexcept { m_EditorMode = mode; }

        inline Quirk::ConstView<EditorTheme>                GetTheme()           const noexcept { return &m_Theme;           }
        inline Quirk::ConstView<EditorFrameResourceManager> GetResourceManager() const noexcept { return &m_ResourceManager; }

    private:
        inline Quirk::WindowSpecification GetEditorFrameWindowSpec() const noexcept {
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
        Quirk::Scope<Quirk::Project> m_Project;
        EditorMode                   m_EditorMode;
        EditorTheme                  m_Theme;
        EditorFrameResourceManager   m_ResourceManager;
	};

}
