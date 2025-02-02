
#pragma once

#include "Core/Core.h"

#include "Project.h"
#include <yaml-cpp/yaml.h>

namespace Quirk {

	class ProjectSerializer {
	public:
		ProjectSerializer(Ref<Project> project);

		bool Serialize(const std::filesystem::path& filepath);
		bool Deserialize(const std::filesystem::path& filepath);

	private:
		Ref<Project> m_Project;
	};

}
