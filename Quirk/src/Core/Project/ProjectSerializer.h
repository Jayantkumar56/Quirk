
#pragma once

#include "Core/Core.h"

#include <yaml-cpp/yaml.h>
#include <filesystem>

namespace Quirk {

    class  EditorProject;
    struct ProjectConfig;

	class EditorProjectSerializer {
	public:
		static bool Serialize(Ref<EditorProject> project, const std::filesystem::path& filepath);
        static bool DeserializeConfig(ProjectConfig& config, const std::filesystem::path& filepath);
	};

}
