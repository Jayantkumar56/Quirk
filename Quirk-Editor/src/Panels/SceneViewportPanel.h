

#pragma once

#include "Quirk.h"

namespace Quirk {

	class SceneViewportPanel {
	public:
		SceneViewportPanel(const Ref<Scene>& scene, uint16_t width = 890, uint16_t height = 800);
		~SceneViewportPanel() = default;

		bool OnEvent(Event& event);
		void OnUpdate();
		void OnImguiUiUpdate();

		uint16_t GetWidth()						const { return m_PanelWidth; }
		uint16_t GetHeight()					const { return m_PanelHeight; }
		bool IsInFocus()						const { return m_IsInFocus; }
		const RendererStats& GetRendererStats()	const { return m_RendererStats; }

	private:
		void CheckAndHandleResize();
		void RenderViewport();

	private:
		uint16_t			m_PanelWidth, m_PanelHeight;
		bool				m_IsInFocus;
		Ref<FrameBuffer>	m_Frame;
		Ref<Scene>			m_MainScene;
		RendererStats		m_RendererStats;
	};

}
