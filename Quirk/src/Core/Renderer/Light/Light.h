

#pragma once

#include "glm/glm.hpp"

namespace Quirk {

	enum class LightType {
		None, Point, Directional, Spot
	};

	struct Material {
		glm::vec3 Ambient;
		glm::vec3 Diffuse;
		glm::vec3 Specular;
		float Shininess;
	};

	std::string_view LightTypeToString(LightType type);
	LightType        StringToLightType(std::string_view type);

}
