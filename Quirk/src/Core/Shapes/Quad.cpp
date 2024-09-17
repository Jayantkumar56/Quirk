

#include "Qkpch.h"
#include "Quad.h"
#include <glm/gtc/matrix_transform.hpp>

namespace Quirk {

	Quad::Quad(const QuadFormat& format):
			m_Position(format.Position),
			m_Rotation(format.Rotation),
			m_TilingFactor(format.TilingFactor),
			m_Color(format.Color),
			m_Texture(format.Texture2d)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position) 
			* glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation), { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { format.Width, format.Height, 1.0f });

		m_Vertices[0] = glm::vec4(-0.5f, -0.5f, 0.0f, 1.0f);
		m_Vertices[1] = glm::vec4( 0.5f, -0.5f, 0.0f, 1.0f);
		m_Vertices[2] = glm::vec4( 0.5f,  0.5f, 0.0f, 1.0f);
		m_Vertices[3] = glm::vec4(-0.5f,  0.5f, 0.0f, 1.0f);
	}

}

