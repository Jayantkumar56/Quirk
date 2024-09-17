

#pragma once

#include "glm/glm.hpp"
#include "glm/ext.hpp"
#include <glm/gtc/matrix_transform.hpp>

namespace Quirk {

	class OrthographicCamera {
	public:
		OrthographicCamera(float left, float right, float bottom, float top);

		inline glm::mat4& GetProjectionMatrix()		{ return m_ProjectionMatrix; }
		inline glm::mat4& GetViewMatrix()			{ return m_ViewMatrix; }
		inline glm::mat4& GetProjectionViewMatrix() { return m_ProjectionViewMatrix; }

		inline void SetProjectionMatrix(float left, float right, float bottom, float top) {
			m_ProjectionMatrix = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
			m_ProjectionViewMatrix = m_ProjectionMatrix * m_ViewMatrix;
		}

		void SetViewMatrix(const glm::vec3& position, float angle);

	private:
		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ProjectionViewMatrix;
	};

}
