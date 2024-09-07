


#include "Qkpch.h"
#include "Core/Camera/CameraController.h"

#include "Core/Utility/Time.h"
#include "Core/Input/Input.h"
#include "Core/Input/KeyCodes.h"

namespace Quirk {

	PerspectiveCameraController::PerspectiveCameraController(float fov, float aspectRatio, float nearPlane, float farPlane) :
			m_ZoomLevel(1.0f),
			m_CameraTranslationSpeed(5.0f), 
			m_CameraRotationSpeed(50.0f),
			m_Pitch				(0.0f),
			m_Yaw				(-90.0f),
			m_PrevCameraPosition(0.0f, 0.0f, 3.0f),
			m_CameraPosition	(0.0f, 0.0f, 3.0f),
			m_ViewFront			(0.0f, 0.0f, -1.0f),
			m_ViewUp			(0.0f, 1.0f, 0.0f),
			m_MovementFront		(0.0f, 0.0f, -1.0f),
			m_MovementRight		(1.0f, 0.0f, 0.0f),
			m_Camera			(glm::radians(fov), aspectRatio, nearPlane, farPlane, m_CameraPosition, m_ViewFront, m_ViewUp)
	{
	}

	void PerspectiveCameraController::OnUpdate() {
		double deltaTime = Quirk::Time::GetDeltaTime();
		double cameraDisplacement = m_CameraTranslationSpeed * deltaTime;

		if (Quirk::Input::IsKeyPressed(QK_Key_S)) {
			m_CameraPosition -= static_cast<float>(cameraDisplacement) * m_MovementFront;
		}
		if (Quirk::Input::IsKeyPressed(QK_Key_W)) {
			m_CameraPosition += static_cast<float>(cameraDisplacement) * m_MovementFront;
		}
		if (Quirk::Input::IsKeyPressed(QK_Key_A)) {
			m_CameraPosition -= static_cast<float>(cameraDisplacement) * m_MovementRight;
		}
		if (Quirk::Input::IsKeyPressed(QK_Key_D)) {
			m_CameraPosition += static_cast<float>(cameraDisplacement) * m_MovementRight;
		}
		if (Quirk::Input::IsKeyPressed(QK_Key_LeftCtrl)) {
			m_CameraPosition -= static_cast<float>(cameraDisplacement) * m_ViewUp;
		}
		if (Quirk::Input::IsKeyPressed(QK_Key_Space)) {
			m_CameraPosition += static_cast<float>(cameraDisplacement) * m_ViewUp;
		}

		if (m_CameraPosition != m_PrevCameraPosition) {
			m_Camera.UpdateViewMatrix(m_CameraPosition, m_ViewFront, m_ViewUp);
			m_PrevCameraPosition = m_CameraPosition;
		}
	}

	void PerspectiveCameraController::OnEvent(Event& e) {
		EventDispatcher::Dispatch<MouseMovedEvent>(QK_BIND_EVENT_FN(PerspectiveCameraController::OnMouseMove));
	}

	void PerspectiveCameraController::OnResize(float width, float height) {
	}

	bool PerspectiveCameraController::OnMouseMove(MouseMovedEvent& e) {
		double deltaTime = Quirk::Time::GetDeltaTime();
		m_Yaw += static_cast<float>(e.GetDeltaX() * m_CameraRotationSpeed * deltaTime);
		m_Pitch += static_cast<float>(e.GetDeltaY() * m_CameraRotationSpeed * deltaTime);

		if (m_Pitch > 89.0f)
			m_Pitch = 89.0f;
		if (m_Pitch < -89.0f)
			m_Pitch = -89.0f;

		if (m_Yaw > 360.0f)
			m_Yaw -= 360.0f;
		if (m_Yaw < -360.0f)
			m_Yaw += 360.0f;

		m_MovementRight.x = glm::cos(glm::radians(m_Yaw + 90.0f));
		m_MovementRight.z = glm::sin(glm::radians(m_Yaw + 90.0f));

		m_MovementFront.x = glm::cos(glm::radians(m_Yaw));
		m_MovementFront.z = glm::sin(glm::radians(m_Yaw));

		glm::vec3 front;
		front.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
		front.y = sin(glm::radians(m_Pitch));
		front.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
		m_ViewFront = glm::normalize(front);

		m_Camera.UpdateViewMatrix(m_CameraPosition, m_ViewFront, m_ViewUp);

		return false;
	}

	bool PerspectiveCameraController::OnWindowResized(WindowResizeEvent& e) {
		return false;
	}

}

