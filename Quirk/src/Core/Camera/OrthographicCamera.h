

#pragma once

#include "glm/glm.hpp"
#include "glm/ext.hpp"

namespace Quirk {

	class OrthographicCamera {
	public:
		OrthographicCamera(float left, float right, float top, float bottom);

		inline glm::vec3& GetPosition() { return m_Position; }
		void SetPosition(const glm::vec3& position);

		inline float GetRotation() const { return m_RotationAngle; }
		void SetRotationAngle(float angle);

		inline glm::mat4& GetProjectionViewMatrix() { return m_ProjectionViewMatrix; }

	private:
		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ProjectionViewMatrix;

		glm::vec3 m_Position;
		float m_RotationAngle;
	};

}
