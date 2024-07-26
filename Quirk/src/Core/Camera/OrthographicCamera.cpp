
#include "Qkpch.h"
#include "OrthographicCamera.h"

namespace Quirk {

	Quirk::OrthographicCamera::OrthographicCamera(float left, float right, float top, float bottom) :
			m_ProjectionMatrix(glm::ortho(left, right, bottom, top, -1.0f, 1.0f)),
			m_ViewMatrix(1.0f),
			m_ProjectionViewMatrix(),
			m_Position(0.0f), 
			m_RotationAngle(0)
	{
		m_ProjectionViewMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void OrthographicCamera::SetPosition(const glm::vec3& position) {
		m_Position = position;

		m_ViewMatrix = glm::translate(glm::mat4(1.0f), position);
		m_ViewMatrix *= glm::rotate(glm::mat4(1.0f), glm::radians(m_RotationAngle), glm::vec3(0.0f, 0.0f, 1.0f));

		m_ViewMatrix = glm::inverse(m_ViewMatrix);
		m_ProjectionViewMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void OrthographicCamera::SetRotationAngle(float angle) {
		m_RotationAngle = angle;

		m_ViewMatrix = glm::translate(glm::mat4(1.0f), m_Position);
		m_ViewMatrix *= glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(0.0f, 0.0f, 1.0f));

		m_ViewMatrix = glm::inverse(m_ViewMatrix);
		m_ProjectionViewMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

}
