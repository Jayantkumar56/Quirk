

#include "Qkpch.h"
#include "ProjectSerializer.h"
#include "Project.h"

#include <fstream>

namespace Quirk {

	bool ProjectSerializer::Serialize(Ref<Project> project, const std::filesystem::path& filePath) {
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

	bool ProjectSerializer::Deserialize(Ref<Project> project, const std::filesystem::path& filePath) {
		auto& config = project->GetConfig();

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

	bool ProjectSerializer::SerializeRecentProjectsList(std::vector<ProjectMetadata>& list, const std::filesystem::path& filePath) {
		YAML::Emitter out;

		out << YAML::BeginMap; // root
		out << YAML::Key << "Recent Projects";
		{
			out << YAML::BeginMap;	// Recent Projects
			out << YAML::Key << "Projects Count" << YAML::Value << list.size();

			out << YAML::Key << "Projects" << YAML::BeginSeq;  // projects sequence 
			for (auto& project : list) {
				{
					out << YAML::BeginMap;  // Project
					out << YAML::Key << "Title" << YAML::Value << project.Title;
					out << YAML::Key << "Path"  << YAML::Value << project.Path.string();
					out << YAML::EndMap;	// Project
				}
			}

			out << YAML::EndSeq;    // projects sequence 
			out << YAML::EndMap;	// Recent Projects
		}

		out << YAML::EndMap;   // root

		std::ofstream fout(filePath);
		fout << out.c_str();

		return true;
	}

	bool ProjectSerializer::DeserializeRecentProjectsList(std::vector<ProjectMetadata>& list, const std::filesystem::path& filePath) {
		YAML::Node data;
		try {
			data = YAML::LoadFile(filePath.string());
		}
		catch (YAML::ParserException e) {
			QK_CORE_ERROR("Failed to load project file");
			return false;
		}

		auto recentProjects = data["Recent Projects"];
		auto projectCount   = recentProjects["Projects Count"].as<int>();

		// clearing the recent list for any item already in the list
		// and reserving appropriate size for the list
		list.clear();
		list.reserve(projectCount);

		// storing all the projects metadata in the list
		auto projects = recentProjects["Projects"];
		for (auto project : projects) {
			list.emplace_back(project["Title"].as<std::string>(), project["Path"].as<std::string>());
		}

		return true;
    }

}
