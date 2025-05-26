

#pragma once

#include "Quirk.h"
#include "Editor/EditorCamera.h"

namespace QuirkEditor {

	class SceneViewportPanel : public Quirk::Panel {
	public:
		SceneViewportPanel(uint16_t width = 1, uint16_t height = 1);

		virtual void OnUpdate()                   override;
		virtual bool OnEvent(Quirk::Event& event) override;
		virtual void SetUiProperties()            override;
		virtual void UnsetUiProperties()          override;
		virtual void OnUiUpdate()                 override;

	private:
		void MenuBar              (const Quirk::Ref<Quirk::Scene>& scene);
		void CheckAndHandleResize (const Quirk::Ref<Quirk::Scene>& scene);
		void RenderViewport       (const Quirk::Ref<Quirk::Scene>& scene);
		int  GetEntityIdOnClick   (const ImVec2& imagePos               );

		void OnSceneEdit(const Quirk::Ref<Quirk::Scene>& scene);
		void OnScenePlay(const Quirk::Ref<Quirk::Scene>& scene);

	private:
        uint16_t m_PanelWidth;
        uint16_t m_PanelHeight;
		bool     m_IsInFocus;

        Quirk::Ref<Quirk::FrameBuffer> m_Frame;

		bool         m_ControllingCamera;
		EditorCamera m_Camera;

		Quirk::Ref<Quirk::Scene> m_RuntimeScene;
	};

}
