

#pragma once
#include "glm/glm.hpp"

namespace Quirk {

	struct QuadVertex {
		glm::vec3 Position;

		// 0000 0000  0000 0000  0000 0000  0000 0000
		// |<- r ->|  |<- g ->|  |<- b ->|  |<- a ->|
		// use SetColor to assign value
		int Color = 0xFFFFFFFF;

		int		  TextureSlot;
		glm::vec2 TextureCoords;

		void SetColor(int r, int g, int b, int a) {
			Color = 0;
			Color |= (r & 0xFF);
			Color |= (g & 0xFF) << 8;
			Color |= (b & 0xFF) << 16;
			Color |= (a & 0xFF) << 24;
		}
	};

}
