

#pragma once

#include "glm/glm.hpp"
#include "Core/Renderer/Texture.h"
#include "Core/Core.h"

namespace Quirk {

	struct QuadFormat {
		glm::vec3 Position;
		uint32_t Width;
		uint32_t Height;
		float Rotation		= 0.0f;
		float TilingFactor	= 1.0f;
		glm::vec4 Color		= {1.0f, 1.0f, 1.0f, 1.0f};
		Ref<Texture2D> Texture2d;
	};

	class Quad {
	public:
		Quad(const QuadFormat& format);
		~Quad() = default;

		inline glm::vec3& GetPosition()				 { return m_Position; }
		inline float GetRotation()			   const { return m_Rotation; }
		inline Ref<Texture2D> GetTexture()			 { return m_Texture; }
		inline glm::vec2* GetVertices()				 { return m_Vertices; }

	private:
		glm::vec3 m_Position;
		float m_Rotation;
		Ref<Texture2D> m_Texture;

		// vertices are stored from bottom left to top left in anticlock wise order
		// thus m_Vertices[0] = bottom left vertex
		//std::vector<glm::vec2> m_Vertices;
		glm::vec2 m_Vertices[4];
	};

}

