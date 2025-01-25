

#pragma once

#include "Quirk.h"
#include "TitleBar.h"
#include "Panels/SceneViewportPanel.h"
#include "Panels/SceneHierarchyPanel.h"
#include "Panels/InspectorPanel.h"
#include "Panels/ContentBrowserPanel.h"

namespace Quirk {

	class EditorFrame : public Frame {
	public:
		EditorFrame(WindowSpecification& spec) :
				Frame(spec),
				m_MainScene(CreateRef<Scene>(0, 0)),
				m_SelectedEntity()
		{
			SetTitleBar<EditorTitleBar>();

			AddPanel<SceneViewportPanel>();
			AddPanel<SceneHierarchyPanel>();
			AddPanel<InspectorPanel>();
			AddPanel<ContentBrowserPanel>("assets");

			SetVSync(true);
		}

		~EditorFrame() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		virtual bool OnEvent(Event& event) override;
		virtual void OnUpdate()			   override;
		virtual void OnImguiUiUpdate()     override;

		inline Ref<Scene>& GetMainScene()      { return m_MainScene;      }
		inline Entity&     GetSelectedEntity() { return m_SelectedEntity; }

	private:
		Ref<Scene> m_MainScene;
		Entity m_SelectedEntity;
	};

}
