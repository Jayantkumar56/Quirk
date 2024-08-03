

#pragma once

#include "Core/Camera/PerspectiveCamera.h"
#include "Core/Input/Events.h"

#include "Core/Input/MouseEvents.h"
#include "Core/Input/ApplicationEvents.h"

namespace Quirk {

	class PerspectiveCameraController {
	public:
		PerspectiveCameraController(float fov, float aspectRatio, float nearPlane, float farPlane);

		void OnUpdate();
		void OnEvent(Event& e);

		void OnResize(float width, float height);

		PerspectiveCamera& GetCamera() { return m_Camera; }
		glm::vec3& GetPosition() { return m_CameraPosition; }

		float GetZoomLevel() const { return m_ZoomLevel; }
		void SetZoomLevel(float level) { m_ZoomLevel = level; }

	private:
		bool OnMouseMove(MouseMovedEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);

	private:
		float m_AspectRatio;
		float m_ZoomLevel;
		PerspectiveCamera m_Camera;

		glm::vec3 m_Rotation;
		glm::vec3 m_CameraPosition;
		glm::vec3 m_PrevCameraPosition;

		float m_CameraTranslationSpeed, m_CameraRotationSpeed;
	};

}
