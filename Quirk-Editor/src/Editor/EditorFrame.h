

#pragma once


#include "Panels/SceneViewportPanel.h"
#include "Panels/SceneHierarchyPanel.h"
#include "Panels/InspectorPanel/InspectorPanel.h"
#include "Panels/ContentBrowserPanel.h"

#include "EditorTitleBar.h"
#include "EditorTheme.h"
#include "EditorResourceManager.h"

#include "Quirk.h"



namespace QuirkEditor {

	class EditorFrame : 
        public Quirk::Frame<
            Quirk::PanelPolicy::Enabled, 
            Quirk::TitleBarPolicy::Enabled
        >
    {
	public:
		EditorFrame(Quirk::WindowSpecification& spec) :
				Frame            (spec),
				m_SelectedEntity (    ),
				m_MainScene      (Quirk::CreateRef<Quirk::Scene>("New Scene", 0, 0))
		{
			Quirk::Renderer::InitRenderer();
			Quirk::Renderer2D::InitRenderer();

            m_Theme.SetTheme(ThemeName::DarkTheme);

			SetTitleBar<EditorTitleBar>(this);

			AddPanel<SceneViewportPanel> (this);
			AddPanel<SceneHierarchyPanel>(this);
			AddPanel<InspectorPanel>     (this);
			AddPanel<ContentBrowserPanel>(this);
		}

		virtual void OnImguiUiUpdate() override {
			// Disabling alt key for imgui to prevent navigation with alt key (problems when using editor cotrols)
			ImGui::SetKeyOwner(ImGuiKey_LeftAlt, ImGuiKeyOwner_Any, ImGuiInputFlags_LockThisFrame);
		}

		inline Quirk::Ref<Quirk::Scene>&  GetMainScene()      { return m_MainScene;      }
		inline Quirk::Entity&             GetSelectedEntity() { return m_SelectedEntity; }

        inline EditorTheme&           GetTheme()           noexcept { return m_Theme;           }
        inline EditorResourceManager& GetResourceManager() noexcept { return m_ResourceManager; }

	private:
		Quirk::Entity     m_SelectedEntity;
		Quirk::Ref<Quirk::Scene> m_MainScene;

        EditorTheme           m_Theme;
        EditorResourceManager m_ResourceManager;
	};

}
