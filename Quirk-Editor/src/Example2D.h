#pragma once

#include "Quirk.h"


class Example2DLayer : public Quirk::Layer {
public:
	Example2DLayer(const std::string& name = "Example2D");

	virtual ~Example2DLayer() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	virtual bool OnEvent(Quirk::Event& event) override;
	virtual void OnImguiUiUpdate() override;
	virtual void OnUpdate() override;

private:
	Quirk::OrthographicCameraController m_CameraController;
	std::vector< Quirk::Ref<Quirk::Quad> > m_Rectangle;
	Quirk::RendererStats m_RendererStats;
	float m_CameraSpeed = 3.0f;
};
