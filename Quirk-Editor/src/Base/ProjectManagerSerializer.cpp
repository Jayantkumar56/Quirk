

#include "QkEditorpch.h"

#include "ProjectManagerSerializer.h"
#include "ProjectManager.h"

#include "Core/Core.h"
#include "yaml-cpp/yaml.h"

namespace QuirkEditor {

    bool ProjectManagerSerializer::SerializeRecentProjectsList(std::vector<ProjectMetadata>& list, const std::filesystem::path& filePath) {
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
                    out << YAML::Key << "Path" << YAML::Value << project.ProjectRootDirectory.string();
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

    bool ProjectManagerSerializer::DeserializeRecentProjectsList(std::vector<ProjectMetadata>& list, const std::filesystem::path& filePath) {
        YAML::Node data;
        try {
            data = YAML::LoadFile(filePath.string());
        }
        catch (YAML::ParserException e) {
            QK_CORE_ERROR("Failed to load project file");
            return false;
        }

        auto recentProjects = data["Recent Projects"];
        auto projectCount = recentProjects["Projects Count"].as<int>();

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