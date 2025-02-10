

#pragma once
#include "glm/glm.hpp"

namespace Quirk {

	enum class PrimitiveMeshType {
		Plane,
		Cube,
		Sphere,
		Cylinder,
		Capsule
	};

	struct PrimitiveSpecification {
		PrimitiveMeshType Type;
		uint32_t   Segments[3] { 1, 1, 1 };
		glm::vec3  Size        { 1, 1, 1 };
		uint32_t   LOD         { 0 };
	};

	struct GeometryImportSettings {

	};

	struct GeometryData {

	};

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
