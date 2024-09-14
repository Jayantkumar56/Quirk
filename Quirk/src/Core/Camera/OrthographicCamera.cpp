
#include "Qkpch.h"
#include "OrthographicCamera.h"

namespace Quirk {

	OrthographicCamera::OrthographicCamera(float left, float right, float top, float bottom) :
			m_ProjectionMatrix(glm::ortho(left, right, bottom, top, -1.0f, 1.0f)),
			m_ViewMatrix(1.0f),
			m_ProjectionViewMatrix(m_ProjectionMatrix* m_ViewMatrix)
	{
	}

	void OrthographicCamera::SetViewMatrix(const glm::vec3& position, float angle) {
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
			glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(0, 0, 1));

		m_ViewMatrix = glm::inverse(transform);
		m_ProjectionViewMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

}
