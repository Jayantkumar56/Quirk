

#pragma once

#include "Geometry.h"
#include "Core/Core.h"

namespace Quirk {

	class PrimitiveMeshGenerator {
	public:
		static Mesh Generate(MeshType type) {
			switch (type) {
				case Quirk::MeshType::Cube:  return GenerateCube();
					
				case Quirk::MeshType::Plane:
				case Quirk::MeshType::Sphere:	 
				case Quirk::MeshType::Cylinder:	 
				case Quirk::MeshType::Capsule:	 
				case Quirk::MeshType::Custom:	 
					QK_CORE_WARN("Generator not created yet for {0}", static_cast<int>(type));
			}

			return Mesh();
		}

	private:
		static Mesh GenerateCube();
	};

}
