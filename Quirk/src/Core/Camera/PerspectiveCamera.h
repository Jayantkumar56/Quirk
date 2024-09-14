

#pragma once

#include "glm/glm.hpp"
#include "glm/ext.hpp"

namespace Quirk {

	class PerspectiveCamera {
	public:
		PerspectiveCamera(float fov, float aspectRatio, float nearPlane, float farPlane, glm::vec3& pos, glm::vec3& front, glm::vec3& up);

		inline float GetFOV()					const { return m_FOV; }
		inline float GetAspectRatio()			const { return m_AspectRatio; }
		inline float GetNearPlane()				const { return m_NearPlane; }
		inline float GetFarPlane()				const { return m_FarPlane; }
		inline glm::mat4& GetProjectionMatrix() { return m_ProjectionMatrix; }
		inline glm::mat4& GetViewMatrix()		{ return m_ViewMatrix; }

		inline glm::mat4& GetProjectionViewMatrix() { 
			m_ProjectionViewMatrix = m_ProjectionMatrix * m_ViewMatrix; 
			return m_ProjectionViewMatrix;
		}

		inline void SetFOV(float fov)				  { m_FOV		  = fov;			RecalculateProjectionMatrix(); }
		inline void SetAspectRatio(float aspectRatio) { m_AspectRatio = aspectRatio;	RecalculateProjectionMatrix(); }
		inline void SetNearPlane(float nearplane)	  { m_NearPlane	  = nearplane;		RecalculateProjectionMatrix(); }
		inline void SetFarPlane(float farplane)		  { m_FarPlane	  = farplane;		RecalculateProjectionMatrix(); }

		inline void RecalculateProjectionMatrix() { 
			m_ProjectionMatrix = glm::perspective(m_FOV, m_AspectRatio, m_NearPlane, m_FarPlane); 
		}
		inline void UpdateViewMatrix(glm::vec3& pos, glm::vec3& front, glm::vec3& up) { 
			m_ViewMatrix = glm::lookAt(pos, front + pos, up); 
		}

	private:
		float m_FOV;
		float m_AspectRatio;
		float m_NearPlane, m_FarPlane;

		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ProjectionViewMatrix;
	};

}

