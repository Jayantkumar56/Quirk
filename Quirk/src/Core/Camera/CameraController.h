

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

		PerspectiveCamera& GetCamera() { return m_Camera; }
		glm::vec3& GetPosition() { return m_CameraPosition; }

		float GetZoomLevel() const { return m_ZoomLevel; }
		void SetZoomLevel(float level) { m_ZoomLevel = level; }

		inline float GetPitch() const { return m_Pitch; }
		inline void SetPitch(float pitch) { m_Pitch = pitch;	m_Camera.UpdateViewMatrix(m_CameraPosition, m_ViewFront, m_ViewUp); }

		inline float GetYaw() const { return m_Yaw; }
		inline void SetYaw(float yaw) { m_Yaw = yaw;	m_Camera.UpdateViewMatrix(m_CameraPosition, m_ViewFront, m_ViewUp); }

	private:
		bool OnMouseMove(MouseMovedEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);

	private:
		float m_ZoomLevel;
		float m_CameraTranslationSpeed, m_CameraRotationSpeed;
		float m_Pitch, m_Yaw;

		glm::vec3 m_PrevCameraPosition;
		glm::vec3 m_CameraPosition;

		glm::vec3 m_ViewFront;
		glm::vec3 m_ViewUp;

		glm::vec3 m_MovementFront;
		glm::vec3 m_MovementRight;

		PerspectiveCamera m_Camera;
	};

}
