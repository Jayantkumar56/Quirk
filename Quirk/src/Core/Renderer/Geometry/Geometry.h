

#pragma once


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
		std::vector<float> Positions;
	};

	struct Mesh {
		MeshType Type = MeshType::None;
		MeshData Data;
	};

}
