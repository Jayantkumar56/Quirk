

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

	struct MeshData {
		std::vector<float> Positions;
	};

	struct Mesh {
		MeshType Type = MeshType::None;
		MeshData Data;
	};

}
