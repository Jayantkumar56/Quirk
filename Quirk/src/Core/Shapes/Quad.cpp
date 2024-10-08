

#include "Qkpch.h"
#include "Quad.h"
#include <glm/gtc/matrix_transform.hpp>
#include "Core/Application/Application.h"

namespace Quirk {

	Quad::Quad(const QuadFormat& format):
			m_Position(format.Position),
			m_Rotation(format.Rotation),
			m_TilingFactor(format.TilingFactor),
			m_Color(format.Color),
			m_Texture(format.Texture2d)
	{
		auto& window = Application::Get().GetWindow();
		float multiplier = 2 / static_cast<float>(window.GetHeight());

		float width  = (multiplier * format.Width);
		float height = (multiplier * format.Height);

		m_Position.x = multiplier * m_Position.x;
		m_Position.y = multiplier * m_Position.y;

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position) 
			* glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation), { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), glm::vec3(width, height, 1.0f));

		m_Vertices[0] = transform * glm::vec4(-0.5, -0.5, 0.0f, 1.0f);
		m_Vertices[1] = transform * glm::vec4( 0.5, -0.5, 0.0f, 1.0f);
		m_Vertices[2] = transform * glm::vec4( 0.5,  0.5, 0.0f, 1.0f);
		m_Vertices[3] = transform * glm::vec4(-0.5,  0.5, 0.0f, 1.0f);
	}

}

