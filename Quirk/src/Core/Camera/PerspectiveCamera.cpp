

#include "Qkpch.h"
#include "PerspectiveCamera.h"

namespace Quirk {

	PerspectiveCamera::PerspectiveCamera(float fov, float aspectRatio, float nearPlane, float farPlane) :
			m_ProjectionMatrix(glm::perspective(fov, aspectRatio, nearPlane, farPlane)),
			m_ViewMatrix(glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp)),
			m_ProjectionViewMatrix(m_ProjectionMatrix * m_ViewMatrix)
	{
		float Yaw = 90.0f;
		float Pitch = 0.0f;

		cameraFront.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		cameraFront.y = sin(glm::radians(Pitch));
		cameraFront.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		cameraFront = glm::normalize(cameraFront);

		m_ViewMatrix = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		m_ProjectionViewMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void PerspectiveCamera::UpdatePosition(const glm::vec3& newPos, const glm::vec3& prevPos) {
		m_ViewMatrix = glm::translate(glm::mat4(1.0f), newPos - prevPos) * m_ViewMatrix;
		m_ProjectionViewMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void PerspectiveCamera::UpdateRotation(float angle, glm::vec3 axis) {
		m_ViewMatrix = glm::rotate(glm::mat4(1.0f), angle, axis) * m_ViewMatrix;
		m_ProjectionViewMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

}

