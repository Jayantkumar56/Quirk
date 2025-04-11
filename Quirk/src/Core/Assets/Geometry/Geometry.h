

#pragma once

#include "glm/glm.hpp"

namespace Quirk {

	enum class MeshType {
		None = 0,
		Cube,
		Plane,
		Sphere,
		Cylinder,
		Capsule,
		Custom
	};

	std::string_view MeshTypeToString(MeshType type);
	MeshType         StringToMeshType(std::string_view type);

	struct MeshData {
		std::vector<glm::vec3> Positions;
		std::vector<glm::vec3> Normals;
		std::vector<glm::vec2> UV;
	};

	struct Mesh {
		MeshType Type = MeshType::None;
		MeshData Data;
	};

}
