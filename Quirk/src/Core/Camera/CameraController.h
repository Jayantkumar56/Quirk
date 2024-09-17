

#pragma once

#include "Core/Camera/PerspectiveCamera.h"
#include "Core/Camera/OrthographicCamera.h"
#include "Core/Input/Events.h"

#include "Core/Input/MouseEvents.h"
#include "Core/Input/ApplicationEvents.h"

namespace Quirk {

	class PerspectiveCameraController {
	public:
		PerspectiveCameraController(float fov, float aspectRatio, float nearPlane, float farPlane);
		~PerspectiveCameraController() = default;

		inline float GetZoomLevel()							const { return m_ZoomLevel; }
		inline float GetTranslationSpeed()					const { return m_CameraTranslationSpeed; }
		inline float GetRotationSpeed()						const { return m_CameraRotationSpeed; }
		inline float GetPitch()								const { return m_Pitch; }
		inline float GetYaw()								const { return m_Yaw; }
		inline const glm::vec3& GetPosition()				{ return m_CameraPosition; }
		inline const glm::vec3& GetPreviousCameraPosition() { return m_PrevCameraPosition; }
		inline PerspectiveCamera& GetCamera()				{ return m_Camera; }

		inline void SetZoomLevel(float level)		 { m_ZoomLevel = level; }
		inline void SetTranslationSpeed(float speed) { m_CameraTranslationSpeed = speed; }
		inline void SetRotationSpeed(float speed)	 { m_CameraRotationSpeed = speed; }
		inline void SetPitch(float pitch)	  { m_Pitch = pitch;	m_Camera.UpdateViewMatrix(m_CameraPosition, m_ViewFront, m_ViewUp); }
		inline void SetYaw(float yaw)		  { m_Yaw = yaw;		m_Camera.UpdateViewMatrix(m_CameraPosition, m_ViewFront, m_ViewUp); }

		inline void SetPosition(glm::vec3 pos){ 
			m_PrevCameraPosition = m_CameraPosition;
			m_CameraPosition = pos;
			m_Camera.UpdateViewMatrix(m_CameraPosition, m_ViewFront, m_ViewUp);
		}

		void OnUpdate();
		void OnEvent(Event& e);

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

	class OrthographicCameraController {
	public:
		OrthographicCameraController(float aspectRatio);
		~OrthographicCameraController() = default;

		inline float GetZoomLevel()							const { return m_ZoomLevel; }
		inline float GetTranslationSpeed()					const { return m_CameraTranslationSpeed; }
		inline float GetRotationSpeed()						const { return m_CameraRotationSpeed; }
		inline float GetRotation()							const { return m_CameraRotation; }
		inline const glm::vec3& GetPosition()				const { return m_CameraPosition; }
		inline const glm::vec3& GetPreviousCameraPosition() const { return m_PrevCameraPosition; }
		inline OrthographicCamera& GetCamera()				{ return m_Camera; }

		inline void SetZoomLevel(float level)		 { m_ZoomLevel = level; }
		inline void SetTranslationSpeed(float speed) { m_CameraTranslationSpeed = speed; }
		inline void SetRotationSpeed(float speed)	 { m_CameraRotationSpeed = speed; }
		inline void SetRotation(float angle)		 { m_CameraRotation = angle;	m_Camera.SetViewMatrix(m_CameraPosition, m_CameraRotation); }

		inline void SetPosition(glm::vec3 pos) {
			m_PrevCameraPosition = m_CameraPosition;
			m_CameraPosition = pos;
			m_Camera.SetViewMatrix(m_CameraPosition, m_CameraRotation);
		}

		void OnUpdate();
		void OnEvent(Event& e);

	private:
		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);

	private:
		float m_ZoomLevel;
		float m_AspectRatio;
		float m_CameraTranslationSpeed, m_CameraRotationSpeed;
		float m_CameraRotation;

		glm::vec3 m_PrevCameraPosition;
		glm::vec3 m_CameraPosition;

		OrthographicCamera m_Camera;
	};

}
