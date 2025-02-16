

#include "Qkpch.h"
#include "Geometry.h"


namespace Quirk {

	std::string_view MeshTypeToString(MeshType type) {
		switch (type) {
			case MeshType::None:       return "None";
			case MeshType::Cube:       return "Cube";
			case MeshType::Plane:      return "Plane";
			case MeshType::Sphere:     return "Sphere";
			case MeshType::Cylinder:   return "Cylinder";
			case MeshType::Capsule:    return "Capsule";
			case MeshType::Custom:     return "Custom";
		}
		return "";
	}

	MeshType StringToMeshType(std::string_view type) {
		if ( type == "Cube"     ) { return MeshType::Cube;     }
		if ( type == "Plane"    ) { return MeshType::Plane;    }
		if ( type == "Sphere"   ) { return MeshType::Sphere;   }
		if ( type == "Cylinder" ) { return MeshType::Cylinder; }
		if ( type == "Capsule"  ) { return MeshType::Capsule;  }
		if ( type == "Custom"   ) { return MeshType::Custom;   }

		return MeshType::None;
	}

}
