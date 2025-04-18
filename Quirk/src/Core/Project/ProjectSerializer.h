
#pragma once

#include "Core/Core.h"

#include <yaml-cpp/yaml.h>
#include <filesystem>

namespace Quirk {

    class Project;
    struct ProjectConfig;

	class ProjectSerializer {
	public:
		static bool Serialize(Ref<Project> project, const std::filesystem::path& filepath);
		static bool Deserialize(Ref<Project> project, const std::filesystem::path& filepath);

        static bool DeserializeConfig(ProjectConfig& config, const std::filesystem::path& filepath);
	};

}
