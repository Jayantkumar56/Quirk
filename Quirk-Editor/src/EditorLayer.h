

#pragma once

#include "Quirk.h"
#include "Panels/SceneViewportPanel.h"
#include "Panels/SceneHierarchyPanel.h"
#include "Panels/InspectorPanel.h"

namespace Quirk {

	class EditorLayer : public Layer {
	public:
		EditorLayer(const std::string& name = "Editor Layer") : 
				Layer(name),
				m_MainScene(std::make_shared<Scene>(890, 800)),
				m_SelectedEntity({}),
				m_SceneViewport(890, 800)
		{}

		~EditorLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		virtual bool OnEvent(Event& event) override;
		virtual void OnUpdate() override;
		virtual void OnImguiUiUpdate() override;

	private:
		Ref<Scene> m_MainScene;
		Entity m_SelectedEntity;

		SceneViewportPanel m_SceneViewport;
		SceneHierarchyPanel m_SceneHierarcy;
		InspectorPanel m_InspectorPanel;
	};

}
