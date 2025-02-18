

#pragma once

#include "Quirk.h"
#include "Editor/EditorCamera.h"

namespace Quirk {

	enum class SceneState {
		Edit = 0, Play
	};

	class SceneViewportPanel : public Panel {
	public:
		SceneViewportPanel(uint16_t width = 1, uint16_t height = 1);
		~SceneViewportPanel() = default;

		virtual void OnUpdate()             override;
		virtual bool OnEvent(Event& event)  override;
		virtual void SetImguiProperties()   override;
		virtual void UnSetImguiProperties() override;
		virtual void OnImguiUiUpdate()      override;

		uint16_t GetWidth()						const { return m_PanelWidth;    }
		uint16_t GetHeight()					const { return m_PanelHeight;   }
		bool     IsInFocus()					const { return m_IsInFocus;     }
		const RendererStats& GetRendererStats()	const { return m_RendererStats; }

	private:
		void MenuBar              (const Ref<Scene>& scene);
		void CheckAndHandleResize (const Ref<Scene>& scene);
		void RenderViewport       (const Ref<Scene>& scene);
		int  GetEntityIdOnClick   (const ImVec2& imagePos);

		void OnSceneEdit(const Ref<Scene>& scene);
		void OnScenePlay(const Ref<Scene>& scene);

	private:
		Ref<Scene>		 m_RuntimeScene;
		Ref<Texture2D>   m_PlayButtonIcon;
		Ref<Texture2D>   m_PauseButtonIcon;
		uint16_t		 m_PanelWidth, m_PanelHeight;
		bool			 m_IsInFocus;
		Ref<FrameBuffer> m_Frame;
		RendererStats	 m_RendererStats;
		bool			 m_ControllingCamera;
		EditorCamera	 m_Camera;
		SceneState		 m_SceneState;
	};

}
