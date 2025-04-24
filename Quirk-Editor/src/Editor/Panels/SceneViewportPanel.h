

#pragma once

#include "Quirk.h"
#include "Editor/EditorCamera.h"

namespace QuirkEditor {

	enum class SceneState {
		Edit = 0, Play
	};

	class SceneViewportPanel : public Quirk::Panel {
	public:
		SceneViewportPanel(uint16_t width = 1, uint16_t height = 1);
		~SceneViewportPanel() = default;

		virtual void OnUpdate()             override;
		virtual bool OnEvent(Quirk::Event& event)  override;
		virtual void SetImguiProperties()   override;
		virtual void UnSetImguiProperties() override;
		virtual void OnImguiUiUpdate()      override;

		uint16_t GetWidth()						const { return m_PanelWidth;    }
		uint16_t GetHeight()					const { return m_PanelHeight;   }
		bool     IsInFocus()					const { return m_IsInFocus;     }
		const Quirk::RendererStats& GetRendererStats()	const { return m_RendererStats; }

	private:
		void MenuBar              (const Quirk::Ref<Quirk::Scene>& scene);
		void CheckAndHandleResize (const Quirk::Ref<Quirk::Scene>& scene);
		void RenderViewport       (const Quirk::Ref<Quirk::Scene>& scene);
		int  GetEntityIdOnClick   (const ImVec2& imagePos);

		void OnSceneEdit(const Quirk::Ref<Quirk::Scene>& scene);
		void OnScenePlay(const Quirk::Ref<Quirk::Scene>& scene);

	private:
		Quirk::Ref<Quirk::Scene>	   m_RuntimeScene;
		Quirk::Ref<Quirk::Texture2D>   m_PlayButtonIcon;
		Quirk::Ref<Quirk::Texture2D>   m_PauseButtonIcon;
		uint16_t		               m_PanelWidth, m_PanelHeight;
		bool			               m_IsInFocus;
        Quirk::Ref<Quirk::FrameBuffer> m_Frame;
        Quirk::RendererStats	       m_RendererStats;
		bool			               m_ControllingCamera;
		EditorCamera	               m_Camera;
		SceneState		               m_SceneState;
	};

}
