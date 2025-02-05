
#pragma once

#include "Core/Core.h"

#include <yaml-cpp/yaml.h>

namespace Quirk {

	class Project;
	struct ProjectMetadata;

	class ProjectSerializer {
	public:
		static bool Serialize(Ref<Project> project, const std::filesystem::path& filepath);
		static bool Deserialize(Ref<Project> project, const std::filesystem::path& filepath);

		static bool SerializeRecentProjectsList(std::vector<ProjectMetadata>& list, const std::filesystem::path& filePath);
		static bool DeserializeRecentProjectsList(std::vector<ProjectMetadata>& list, const std::filesystem::path& filePath);
	};

}
