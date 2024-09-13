

#pragma once

#include "glm/glm.hpp"
#include "glm/ext.hpp"

namespace Quirk {

	class PerspectiveCamera {
	public:
		PerspectiveCamera(float fov, float aspectRatio, float nearPlane, float farPlane, glm::vec3& pos, glm::vec3& front, glm::vec3& up);

		glm::mat4& GetProjectionMatrix() { return m_ProjectionMatrix; }
		glm::mat4& GetViewMatrix() { return m_ViewMatrix; }
		glm::mat4 GetProjectionViewMatrix() { return m_ProjectionMatrix * m_ViewMatrix; }

		void UpdateViewMatrix(glm::vec3& pos, glm::vec3& front, glm::vec3& up);
		void UpdateAspectRatio(float aspectRatio);

	private:
		float m_FOV;
		float m_NearPlane, m_FarPlane;

		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewMatrix;
	};

}

