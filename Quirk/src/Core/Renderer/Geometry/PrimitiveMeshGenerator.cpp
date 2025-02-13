

#include "Qkpch.h"
#include "PrimitiveMeshGenerator.h"


namespace Quirk {

	Mesh PrimitiveMeshGenerator::GenerateCube() {
		Mesh tempMesh{ .Type { MeshType::Cube } };

		tempMesh.Data.Positions = {
			-0.5f,  -0.5f,  -0.5f,
			 0.5f,  -0.5f,  -0.5f,
			 0.5f,   0.5f,  -0.5f,
			 0.5f,   0.5f,  -0.5f,
			-0.5f,   0.5f,  -0.5f,
			-0.5f,  -0.5f,  -0.5f,

			-0.5f,  -0.5f,   0.5f,
			 0.5f,  -0.5f,   0.5f,
			 0.5f,   0.5f,   0.5f,
			 0.5f,   0.5f,   0.5f,
			-0.5f,   0.5f,   0.5f,
			-0.5f,  -0.5f,   0.5f,

			-0.5f,   0.5f,   0.5f,
			-0.5f,   0.5f,  -0.5f,
			-0.5f,  -0.5f,  -0.5f,
			-0.5f,  -0.5f,  -0.5f,
			-0.5f,  -0.5f,   0.5f,
			-0.5f,   0.5f,   0.5f,

			 0.5f,   0.5f,   0.5f,
			 0.5f,   0.5f,  -0.5f,
			 0.5f,  -0.5f,  -0.5f,
			 0.5f,  -0.5f,  -0.5f,
			 0.5f,  -0.5f,   0.5f,
			 0.5f,   0.5f,   0.5f,

			-0.5f,  -0.5f,  -0.5f,
			 0.5f,  -0.5f,  -0.5f,
			 0.5f,  -0.5f,   0.5f,
			 0.5f,  -0.5f,   0.5f,
			-0.5f,  -0.5f,   0.5f,
			-0.5f,  -0.5f,  -0.5f,

			-0.5f,   0.5f,  -0.5f,
			 0.5f,   0.5f,  -0.5f,
			 0.5f,   0.5f,   0.5f,
			 0.5f,   0.5f,   0.5f,
			-0.5f,   0.5f,   0.5f,
			-0.5f,   0.5f,  -0.5f,
		};

		return tempMesh;
	}

}
