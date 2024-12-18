


#include "Qkpch.h"
#include "Core/Camera/CameraController.h"

#include "Core/Utility/Time.h"
#include "Core/Input/Input.h"
#include "Core/Input/KeyCodes.h"

namespace Quirk {

	//////////////////////////////////////////      PerspectiveCameraController      //////////////////////////////////////////////////////////

	PerspectiveCameraController::PerspectiveCameraController(float fov, float aspectRatio, float nearPlane, float farPlane) :
			m_ZoomLevel(1.0f),
			m_CameraTranslationSpeed(5.0f), 
			m_CameraRotationSpeed(0.03f),
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
		RecalculateViewMatrix();
	}

	void PerspectiveCameraController::OnUpdate() {
		double deltaTime = Time::GetDeltaTime();
		double cameraDisplacement = m_CameraTranslationSpeed * deltaTime;

		if (Input::IsKeyPressed(QK_Key_S)) {
			m_CameraPosition -= static_cast<float>(cameraDisplacement) * m_MovementFront;
		}
		if (Input::IsKeyPressed(QK_Key_W)) {
			m_CameraPosition += static_cast<float>(cameraDisplacement) * m_MovementFront;
		}
		if (Input::IsKeyPressed(QK_Key_A)) {
			m_CameraPosition -= static_cast<float>(cameraDisplacement) * m_MovementRight;
		}
		if (Input::IsKeyPressed(QK_Key_D)) {
			m_CameraPosition += static_cast<float>(cameraDisplacement) * m_MovementRight;
		}
		if (Input::IsKeyPressed(QK_Key_LeftCtrl)) {
			m_CameraPosition -= static_cast<float>(cameraDisplacement) * m_ViewUp;
		}
		if (Input::IsKeyPressed(QK_Key_Space)) {
			m_CameraPosition += static_cast<float>(cameraDisplacement) * m_ViewUp;
		}

		if (m_CameraPosition != m_PrevCameraPosition) {
			RecalculateViewMatrix();
			m_PrevCameraPosition = m_CameraPosition;
		}
	}

	void PerspectiveCameraController::OnEvent(Event& e) {
		EventDispatcher::HandleEvent<MouseMovedEvent>(QK_BIND_EVENT_FN(PerspectiveCameraController::OnMouseMove));
		EventDispatcher::HandleEvent<WindowResizeEvent>(QK_BIND_EVENT_FN(PerspectiveCameraController::OnWindowResized));
	}

	bool PerspectiveCameraController::OnMouseMove(MouseMovedEvent& e) {
		m_Yaw += static_cast<float>(e.GetDeltaX() * m_CameraRotationSpeed);
		m_Pitch += static_cast<float>(e.GetDeltaY() * m_CameraRotationSpeed);

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

		RecalculateViewMatrix();

		return false;
	}

	bool PerspectiveCameraController::OnWindowResized(WindowResizeEvent& e) {
		m_Camera.SetAspectRatio(static_cast<float>(e.GetWidth()) / static_cast<float>(e.GetHeight()));
		return false;
	}

	//////////////////////////////////////////      OrthographicCameraController      //////////////////////////////////////////////////////////

	OrthographicCameraController::OrthographicCameraController(float aspectRatio) :
		m_ZoomLevel(1.0f),
		m_AspectRatio(aspectRatio),
		m_CameraTranslationSpeed(5.0f),
		m_CameraRotationSpeed(0.03f),
		m_CameraRotation(0.0f),
		m_PrevCameraPosition(0.0f, 0.0f, 0.0f),
		m_CameraPosition(0.0f, 0.0f, 0.0f),
		m_Camera(-aspectRatio * m_ZoomLevel, aspectRatio * m_ZoomLevel,  -m_ZoomLevel, m_ZoomLevel)
	{
		RecalculateViewMatrix();
	}

	void OrthographicCameraController::OnUpdate() {
		double deltaTime = Time::GetDeltaTime();
		double cameraDisplacement = m_CameraTranslationSpeed * deltaTime;

		if (Input::IsKeyPressed(QK_Key_S)) {
			m_CameraPosition.y -= static_cast<float>(cameraDisplacement);
		}
		if (Input::IsKeyPressed(QK_Key_W)) {
			m_CameraPosition.y += static_cast<float>(cameraDisplacement);
		}
		if (Input::IsKeyPressed(QK_Key_A)) {
			m_CameraPosition.x -= static_cast<float>(cameraDisplacement);
		}
		if (Input::IsKeyPressed(QK_Key_D)) {
			m_CameraPosition.x += static_cast<float>(cameraDisplacement);
		}

		if (m_CameraPosition != m_PrevCameraPosition) {
			m_PrevCameraPosition = m_CameraPosition;
			RecalculateViewMatrix();
		}
	}

	void OrthographicCameraController::OnEvent(Event& e) {
		EventDispatcher::HandleEvent<WindowResizeEvent>(QK_BIND_EVENT_FN(OrthographicCameraController::OnWindowResized));
		EventDispatcher::HandleEvent<MouseScrolledEvent>(QK_BIND_EVENT_FN(OrthographicCameraController::OnMouseScrolled));
	}

	void OrthographicCameraController::HandleWindowResize(uint16_t width, uint16_t height) {
		m_AspectRatio = static_cast<float>(width) / static_cast<float>(height);
		m_Camera.SetProjectionMatrix(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		m_ProjectionView = m_Camera.GetProjection() * m_View;
	}

	bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent& e) {
		m_ZoomLevel -= e.GetOffset() * 0.25f;
		m_ZoomLevel = std::max(m_ZoomLevel, 0.25f);
		m_Camera.SetProjectionMatrix(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		m_ProjectionView = m_Camera.GetProjection() * m_View;
		return false;
	}

	bool OrthographicCameraController::OnWindowResized(WindowResizeEvent& e) {
		m_AspectRatio = static_cast<float>(e.GetWidth()) / static_cast<float>(e.GetHeight());
		m_Camera.SetProjectionMatrix(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		m_ProjectionView = m_Camera.GetProjection() * m_View;
		return false;
	}

	void OrthographicCameraController::RecalculateViewMatrix() {
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_CameraPosition) *
			glm::rotate(glm::mat4(1.0f), glm::radians(m_CameraRotation), glm::vec3(0, 0, 1));

		m_View = glm::inverse(transform);
		m_ProjectionView = m_Camera.GetProjection() * m_View;
	}

}

