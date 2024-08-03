

#pragma once

#include "glm/glm.hpp"
#include "glm/ext.hpp"

namespace Quirk {

	class PerspectiveCamera {
	public:
		PerspectiveCamera(float fov, float aspectRatio, float nearPlane, float farPlane);

		glm::mat4& GetProjectionMatrix() { return m_ProjectionMatrix; }
		glm::mat4& GetViewMatrix() { return m_ViewMatrix; }
		glm::mat4& GetProjectionViewMatrix() { return m_ProjectionViewMatrix; }

		void UpdatePosition(const glm::vec3& newPos, const glm::vec3& prevPos);
		void UpdateRotation(float angle, glm::vec3 axis);

	private:
		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ProjectionViewMatrix;

		glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, -3.0f);
		glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, 1.0f);
		glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	};

}

