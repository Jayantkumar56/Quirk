

#pragma once

#include "Quirk.h"
#include "EditorTitleBar.h"
#include "Panels/SceneViewportPanel.h"
#include "Panels/SceneHierarchyPanel.h"
#include "Panels/InspectorPanel.h"
#include "Panels/ContentBrowserPanel.h"
#include "Theme.h"


namespace Quirk {

	class EditorFrame : public Frame {
	public:
		EditorFrame(WindowSpecification& spec) :
				Frame            (spec),
				m_SelectedEntity (    ),
				m_MainScene      (CreateRef<Scene>(0, 0))
		{
			Renderer::InitRenderer();
			Renderer2D::InitRenderer();
			Theme::SetTheme(ThemeName::DarkTheme);

			SetTitleBar<EditorTitleBar>();

			AddPanel<SceneViewportPanel> ();
			AddPanel<SceneHierarchyPanel>();
			AddPanel<InspectorPanel>     ();
			AddPanel<ContentBrowserPanel>();
		}

		~EditorFrame() = default;

		virtual bool OnEvent(Event& event) override { return false; }
		virtual void OnUpdate()			   override { }

		virtual void OnImguiUiUpdate() override {
			// Disabling alt key for imgui to prevent navigation with alt key (problems when using editor cotrols)
			ImGui::SetKeyOwner(ImGuiKey_LeftAlt, ImGuiKeyOwner_Any, ImGuiInputFlags_LockThisFrame);
		}

		inline Ref<Scene>&  GetMainScene()      { return m_MainScene;      }
		inline Entity&      GetSelectedEntity() { return m_SelectedEntity; }

	private:
		Entity     m_SelectedEntity;
		Ref<Scene> m_MainScene;
	};

}
