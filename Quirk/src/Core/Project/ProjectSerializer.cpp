

#include "Qkpch.h"
#include "ProjectSerializer.h"

#include "EditorProject.h"


#include <fstream>

namespace Quirk {

	bool EditorProjectSerializer::Serialize(Ref<EditorProject> project, const std::filesystem::path& filePath) {
        if (project == nullptr) {
            QK_WARN("No project is provided for Serialization!");
            return false;
        }

		const auto& config = project->GetConfig();

		YAML::Emitter out;
		{
			out << YAML::BeginMap; // Root
			out << YAML::Key << "Project" << YAML::Value;
			{
				out << YAML::BeginMap;// Project
				out << YAML::Key << "Name"              << YAML::Value << config.Name;
				out << YAML::Key << "StartScene"        << YAML::Value << config.StartScene.string();
				out << YAML::Key << "AssetDirectory"    << YAML::Value << config.AssetDirectory.string();
				out << YAML::Key << "SceneDirectory"    << YAML::Value << config.SceneDirectory.string();
				out << YAML::Key << "ScriptModulePath"  << YAML::Value << config.ScriptModulePath.string();
				out << YAML::Key << "AssetRegistryPath" << YAML::Value << config.AssetRegistryPath.string();
				out << YAML::EndMap; // Project
			}
			out << YAML::EndMap; // Root
		}

		std::ofstream fout(filePath);
		fout << out.c_str();

		return true;
	}

    bool EditorProjectSerializer::DeserializeConfig(ProjectConfig& config, const std::filesystem::path& filePath) {
        YAML::Node data;
		try {
			data = YAML::LoadFile(filePath.string());
		}
		catch (YAML::ParserException e) {
			QK_CORE_ERROR("Failed to load project file");
			return false;
		}

		auto projectNode = data["Project"];
		if (!projectNode)
			return false;

		config.Name              = projectNode["Name"             ].as<std::string>();
		config.StartScene        = projectNode["StartScene"       ].as<std::string>();
		config.AssetDirectory    = projectNode["AssetDirectory"   ].as<std::string>();
		config.SceneDirectory    = projectNode["SceneDirectory"   ].as<std::string>();
		config.ScriptModulePath  = projectNode["ScriptModulePath" ].as<std::string>();
		config.AssetRegistryPath = projectNode["AssetRegistryPath"].as<std::string>();
		return true;
    }

}
