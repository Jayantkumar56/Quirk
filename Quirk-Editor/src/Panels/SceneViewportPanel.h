

#pragma once

#include "Quirk.h"

namespace Quirk {

	class SceneViewportPanel {
	public:
		SceneViewportPanel(uint16_t width = 890, uint16_t height = 800);
		~SceneViewportPanel() = default;

		bool OnEvent(Event& event);
		void OnUpdate(const Ref<Scene>& scene);
		void OnImguiUiUpdate(const Ref<Scene>& scene);

		uint16_t GetWidth()						const { return m_PanelWidth; }
		uint16_t GetHeight()					const { return m_PanelHeight; }
		bool IsInFocus()						const { return m_IsInFocus; }
		const RendererStats& GetRendererStats()	const { return m_RendererStats; }

	private:
		void CheckAndHandleResize(const Ref<Scene>& scene);
		void RenderViewport(const Ref<Scene>& scene);

	private:
		uint16_t			m_PanelWidth, m_PanelHeight;
		bool				m_IsInFocus;
		Ref<FrameBuffer>	m_Frame;
		RendererStats		m_RendererStats;
	};

}
