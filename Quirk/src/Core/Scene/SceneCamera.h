

#pragma once
#include "Core/Camera/Camera.h"

namespace Quirk {

	class SceneSerializer;

	class SceneCamera : public Camera {
		friend class SceneSerializer;

	public:
		enum ProjectionType { Perspective = 0, Orthographic = 1 };

	public:
		SceneCamera();
		virtual ~SceneCamera() = default;

		void SetPerspective(float verticalFOV, float nearClip, float farClip);
		void SetOrthographic(float size, float nearClip, float farClip);

		void SetViewportSize(uint32_t width, uint32_t height);

		float GetPerspectiveVerticalFOV()	const { return m_PerspectiveFOV; }
		float GetPerspectiveNearClip()		const { return m_PerspectiveNear; }
		float GetPerspectiveFarClip()		const { return m_PerspectiveFar; }
		void  SetPerspectiveVerticalFOV(float verticalFov)	{ m_PerspectiveFOV  = verticalFov;	RecalculateProjection(); }
		void  SetPerspectiveNearClip(float nearClip)		{ m_PerspectiveNear = nearClip;		RecalculateProjection(); }
		void  SetPerspectiveFarClip(float farClip)			{ m_PerspectiveFar  = farClip;		RecalculateProjection(); }

		float GetOrthographicSize()		const { return m_OrthographicSize; }
		float GetOrthographicNearClip() const { return m_OrthographicNear; }
		float GetOrthographicFarClip()	const { return m_OrthographicFar; }
		void  SetOrthographicSize(float size)			{ m_OrthographicSize = size;		RecalculateProjection(); }
		void  SetOrthographicNearClip(float nearClip)	{ m_OrthographicNear = nearClip;	RecalculateProjection(); }
		void  SetOrthographicFarClip(float farClip)		{ m_OrthographicFar  = farClip;		RecalculateProjection(); }

		ProjectionType GetProjectionType()		  const { return m_ProjectionType; }
		void SetProjectionType(ProjectionType type)		{ m_ProjectionType = type;			RecalculateProjection(); }

	private:
		void RecalculateProjection();

	private:
		ProjectionType m_ProjectionType = ProjectionType::Orthographic;

		float m_PerspectiveFOV  = glm::radians(45.0f);
		float m_PerspectiveNear = 0.01f, m_PerspectiveFar = 1000.0f;

		float m_OrthographicSize = 10.0f;
		float m_OrthographicNear = -1.0f, m_OrthographicFar = 1.0f;

		float m_AspectRatio = 0.0f;
	};

}
