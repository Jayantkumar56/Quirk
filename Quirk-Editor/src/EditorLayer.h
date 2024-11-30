

#pragma once

#include "Quirk.h"
#include "TitleBar.h"
#include "Panels/SceneViewportPanel.h"
#include "Panels/SceneHierarchyPanel.h"
#include "Panels/InspectorPanel.h"
#include "Panels/ContentBrowserPanel.h"

namespace Quirk {

	class EditorLayer : public Layer {
	public:
		EditorLayer(const std::string& name = "Editor Layer") : 
				Layer(name),
				m_MainScene(CreateRef<Scene>(0, 0)),
				m_SelectedEntity(),
				m_SceneViewport(),
				m_ContentBrowser("assets")				// temporarily set to "assets" till we have a proper project setup
		{}

		~EditorLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		virtual bool OnEvent(Event& event) override;
		virtual void OnUpdate()			   override;
		virtual void OnImguiUiUpdate()     override;

	private:
		Ref<Scene> m_MainScene;
		Entity m_SelectedEntity;

		TitleBar m_TitleBar;
		SceneViewportPanel  m_SceneViewport;
		SceneHierarchyPanel m_SceneHierarcy;
		InspectorPanel		m_InspectorPanel;
		ContentBrowserPanel m_ContentBrowser;
	};

}
