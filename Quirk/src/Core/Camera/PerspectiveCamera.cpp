

#include "Qkpch.h"
#include "PerspectiveCamera.h"

namespace Quirk {

	PerspectiveCamera::PerspectiveCamera(float fov, float aspectRatio, float nearPlane, float farPlane, glm::vec3& pos, glm::vec3& front, glm::vec3& up) :
		m_FOV(fov),
		m_AspectRatio(aspectRatio),
		m_NearPlane(nearPlane),
		m_FarPlane(farPlane),
		m_ProjectionMatrix(glm::perspective(fov, aspectRatio, nearPlane, farPlane)),
		m_ViewMatrix(glm::lookAt(pos, pos + front, up)),
		m_ProjectionViewMatrix(0.0f)
	{
	}

}

