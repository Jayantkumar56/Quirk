

#pragma once

#include "Core/Application/Layer.h"
#include "Core/Renderer/FrameBuffer.h"
#include "Core/Camera/CameraController.h"
#include "Core/Shapes/Quad.h"
#include "Core/Renderer/Renderer2D.h"

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

	private:
		Ref<Texture2D>					m_QuadTexture;
		Ref<FrameBuffer>				m_Frame;
		float							m_CameraSpeed;
		OrthographicCameraController	m_CameraController;
		std::vector< Ref<Quad> >		m_Rectangle;
		RendererStats					m_RendererStats;
	};

}

