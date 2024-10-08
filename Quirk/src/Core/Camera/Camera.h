

#pragma once

#include "glm/glm.hpp"
#include "glm/ext.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace Quirk {

	class Camera {
	public:
		Camera() = default;
		Camera(const glm::mat4& projection) : m_Projection(projection) {}
		virtual ~Camera() = default;

		const glm::mat4& GetProjection() const { return m_Projection; }

	protected:
		glm::mat4 m_Projection = glm::mat4(1.0f);
	};

	class OrthographicCamera : public Camera{
	public:
		OrthographicCamera(float left, float right, float bottom, float top) :
			Camera(glm::ortho(left, right, bottom, top, -1.0f, 1.0f))
		{
		}

		inline void SetProjectionMatrix(float left, float right, float bottom, float top) {
			m_Projection = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
		}
	};

	class PerspectiveCamera : public Camera{
	public:
		PerspectiveCamera(float fov, float aspectRatio, float nearPlane, float farPlane, glm::vec3& pos, glm::vec3& front, glm::vec3& up) :
			Camera(glm::perspective(fov, aspectRatio, nearPlane, farPlane)),
			m_FOV(fov),
			m_AspectRatio(aspectRatio),
			m_NearPlane(nearPlane),
			m_FarPlane(farPlane)
		{
		}

		inline float GetFOV()			const { return m_FOV; }
		inline float GetAspectRatio()	const { return m_AspectRatio; }
		inline float GetNearPlane()		const { return m_NearPlane; }
		inline float GetFarPlane()		const { return m_FarPlane; }

		inline void SetFOV(float fov)					{ m_FOV = fov;					RecalculateProjectionMatrix(); }
		inline void SetAspectRatio(float aspectRatio)	{ m_AspectRatio = aspectRatio;	RecalculateProjectionMatrix(); }
		inline void SetNearPlane(float nearplane)		{ m_NearPlane = nearplane;		RecalculateProjectionMatrix(); }
		inline void SetFarPlane(float farplane)			{ m_FarPlane = farplane;		RecalculateProjectionMatrix(); }

		inline void RecalculateProjectionMatrix() {
			m_Projection = glm::perspective(m_FOV, m_AspectRatio, m_NearPlane, m_FarPlane);
		}

	private:
		float m_FOV;
		float m_AspectRatio;
		float m_NearPlane, m_FarPlane;
	};

}
