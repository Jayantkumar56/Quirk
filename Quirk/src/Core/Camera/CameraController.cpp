


#include "Qkpch.h"
#include "Core/Camera/CameraController.h"

#include "Core/Utility/Time.h"
#include "Core/Input/Input.h"
#include "Core/Input/KeyCodes.h"

namespace Quirk {

	PerspectiveCameraController::PerspectiveCameraController(float fov, float aspectRatio, float nearPlane, float farPlane) :
			m_AspectRatio(aspectRatio),
			m_ZoomLevel(1.0f),
			m_Camera(glm::radians(fov), aspectRatio, nearPlane, farPlane),
			m_Rotation(glm::vec3(0.0f)),
			m_CameraPosition(glm::vec3(0.0f)),
			m_PrevCameraPosition(glm::vec3(0.0f)),
			m_CameraTranslationSpeed(5.0f), 
			m_CameraRotationSpeed(10.0f)
	{
	}

	void PerspectiveCameraController::OnUpdate() {
		double deltaTime = Quirk::Time::GetDeltaTime();
		double cameraDisplacement = m_CameraTranslationSpeed * deltaTime;

		if (Quirk::Input::IsKeyPressed(QK_Key_S)) {
			m_CameraPosition.z -= static_cast<float>(cameraDisplacement);
		}
		if (Quirk::Input::IsKeyPressed(QK_Key_W)) {
			m_CameraPosition.z += static_cast<float>(cameraDisplacement);
		}
		if (Quirk::Input::IsKeyPressed(QK_Key_A)) {
			m_CameraPosition.x += static_cast<float>(cameraDisplacement);
		}
		if (Quirk::Input::IsKeyPressed(QK_Key_D)) {
			m_CameraPosition.x -= static_cast<float>(cameraDisplacement);
		}
		if (Quirk::Input::IsKeyPressed(QK_Key_LeftCtrl)) {
			m_CameraPosition.y += static_cast<float>(cameraDisplacement);
		}
		if (Quirk::Input::IsKeyPressed(QK_Key_Space)) {
			m_CameraPosition.y -= static_cast<float>(cameraDisplacement);
		}

		if (m_CameraPosition != m_PrevCameraPosition) {
			m_Camera.UpdatePosition(m_CameraPosition, m_PrevCameraPosition);
			m_PrevCameraPosition = m_CameraPosition;
		}
	}

	void PerspectiveCameraController::OnEvent(Event& e) {
		EventDispatcher::Dispatch<MouseMovedEvent>(QK_BIND_EVENT_FN(PerspectiveCameraController::OnMouseMove));
	}

	void PerspectiveCameraController::OnResize(float width, float height)
	{
	}

	bool PerspectiveCameraController::OnMouseMove(MouseMovedEvent& e) {
		e.GetDeltaX();
		e.GetDeltaY();

		float angle = e.GetDeltaX() * m_CameraRotationSpeed;

		return false;
	}

	bool PerspectiveCameraController::OnWindowResized(WindowResizeEvent& e)
	{
		return false;
	}

}

