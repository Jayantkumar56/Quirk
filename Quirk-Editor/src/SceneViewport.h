

#pragma once

#include "Quirk.h"

namespace Quirk {

	class SceneViewport: public Layer {
	public:
		SceneViewport(const std::string& name = "Scene Viewport");
		virtual ~SceneViewport() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		virtual bool OnEvent(Event& event) override;
		virtual void OnUpdate() override;
		virtual void OnImguiUiUpdate() override;

		const RendererStats& GetRendererStats()					  const { return m_RendererStats; }
		const OrthographicCameraController& GetCameraController() const { return m_CameraController; }
		const bool IsInFocus()									  const { return m_IsInFocus; }

	private:
		void CheckAndHandleResize();
		void RenderViewport();

	private:
		uint16_t						m_ViewportWidth,	m_ViewportHeight;
		bool							m_IsInFocus;
		Ref<Texture2D>					m_QuadTexture;
		Ref<FrameBuffer>				m_Frame;
		float							m_CameraSpeed;
		OrthographicCameraController	m_CameraController;
		std::vector< Ref<Quad> >		m_Rectangle;
		RendererStats					m_RendererStats;
	};

}

