

#pragma once

#include "Quirk.h"


namespace Quirk {

	class Example2DLayer : public Layer {
	public:
		Example2DLayer(const std::string& name = "Example2D");

		virtual ~Example2DLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		virtual bool OnEvent(Event& event) override;
		virtual void OnImguiUiUpdate() override;
		virtual void OnUpdate() override;

	private:
		OrthographicCameraController m_CameraController;
		std::vector< Ref<Quad> > m_Rectangle;
		RendererStats m_RendererStats;
		float m_CameraSpeed = 3.0f;
	};

}

