

#pragma once

#include "Editor/EditorCamera.h"

#include "Core/Frame/Panel.h"
#include "Core/Scene/Scene.h"
#include "Core/Renderer/RendererPrimitives/FrameBuffer.h"
#include "Core/Renderer/EditorSceneRenderer.h"

namespace QuirkEditor {

    class EditorFrame;

	class SceneViewportPanel final : public Quirk::Panel {
	public:
		SceneViewportPanel(EditorFrame* frame);

		virtual void OnUpdate()                   override;
		virtual bool OnEvent(Quirk::Event& event) override;
		virtual void SetUiProperties()            override;
		virtual void UnsetUiProperties()          override;
		virtual void OnUiUpdate()                 override;

	private:
		void MenuBar              (Quirk::View<Quirk::Scene> scene);
		void CheckAndHandleResize (Quirk::View<Quirk::Scene> scene);
		int  GetEntityIdOnClick   (const ImVec2& imagePos         );

	private:
        // dummy size value for initialization untill actual values are fetched in the ui update
        ImVec2 m_PanelSize{250.0f, 350.0f};

		bool m_IsInFocus         = false;
		bool m_ControllingCamera = false;

		EditorCamera m_Camera;

        Quirk::Ref<Quirk::FrameBuffer> m_Frame;
        Quirk::EditorSceneRenderer     m_SceneRenderer;

        Quirk::View<EditorFrame> m_EditorFrame;
	};

}
