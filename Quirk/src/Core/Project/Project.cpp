

#include "Qkpch.h"
#include "Project.h"

namespace Quirk {

	std::filesystem::path Project::AddRecentProject(const std::filesystem::path& directory) {
		// find the proj file :-
		std::filesystem::path projFilePath;
		for (const auto& entry : std::filesystem::directory_iterator(directory)) {
			if (entry.is_regular_file() && entry.path().extension() == ".qkproj") {
				projFilePath = entry;
				break;
			}
		}

		// if not found project file (**.qkproj) return empty ProjectMetadata
		if (projFilePath == "")
			return projFilePath;

		std::string projTitle = projFilePath.stem().string();

		// check if project already exists in the recent list then return 
		for (size_t i = 0; i < s_RecentProjectsList.size(); ++i) {
			if (s_RecentProjectsList[i].Title == projTitle && s_RecentProjectsList[i].Path == directory) {
				// making the currently selected item to be the first in the list
				std::rotate(s_RecentProjectsList.begin(), s_RecentProjectsList.begin() + i, s_RecentProjectsList.begin() + i + 1);
				return projFilePath;
			}
		}

		// not present in the recent list so add to the list in front
		s_RecentProjectsList.emplace(s_RecentProjectsList.begin(), projTitle, directory.string());
		return projFilePath;
	}

	Ref<Project> Project::CreateNewProject(ProjectMetadata& projMetaData) {
		ProjectConfig newConfig{
			.Name              { projMetaData.Title   },
			.StartScene        { ""                   },
			.AssetDirectory    { "Assets"             },
			.SceneDirectory    { "Assets/Scenes"      },
			.ScriptModulePath  { "Scripts"            },
			.AssetRegistryPath { "AssetRegistry.yaml" }
		};

		std::filesystem::path projFilePath = projMetaData.Path / (projMetaData.Title + ".qkproj");

		// main project directory created
		std::filesystem::create_directory(projMetaData.Path);

		// all subdirectories are created
		std::filesystem::create_directory(projMetaData.Path / newConfig.AssetDirectory);
		std::filesystem::create_directory(projMetaData.Path / newConfig.SceneDirectory);
		std::filesystem::create_directory(projMetaData.Path / newConfig.ScriptModulePath);

		// creating the AssetRegistry file
		std::ofstream file(projMetaData.Path / newConfig.AssetRegistryPath);

		// loading the project
		Ref<Project> project = CreateRef<Project>();
		project->m_Config    = newConfig;
		project->m_ProjectDirectory = projMetaData.Path;

		s_ActiveProject = project;

		// saving the .qkproj file
		ProjectSerializer::Serialize(s_ActiveProject, projFilePath);

		return s_ActiveProject;
	}

}
