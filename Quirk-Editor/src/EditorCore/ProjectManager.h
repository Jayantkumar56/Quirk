

#pragma once

#include "ProjectManagerSerializer.h"
#include "Core/AssetManager/EditorAssetManager.h"
#include "Core/Project/Project.h"


namespace Quirk {

	struct ProjectMetadata {
		std::string Title;

        // directory of the **.qkproj file (i.e. the root directory of the project)
		std::filesystem::path ProjectRootDirectory;
	};

    class ProjectManager {
    public:
        // takes path of the yaml file which contains the recent projects list
        static inline bool LoadRecentProjectsList(const std::filesystem::path& filePath) {
            return ProjectManagerSerializer::DeserializeRecentProjectsList(s_RecentProjectsList, filePath);
        }

        // takes path of the file where recent list should be stored
        static inline bool SaveRecentProjectsList(const std::filesystem::path& filePath) {
            return ProjectManagerSerializer::SerializeRecentProjectsList(s_RecentProjectsList, filePath);
        }

        static inline const std::vector<ProjectMetadata>& GetRecentProjectsList() noexcept { return s_RecentProjectsList; }

        // projectDirectory is the directory where project root directory will be created
        static Ref<Project> CreateInDirectory(std::string&& title, const std::filesystem::path& projDirectory);

        // takes in path of the **.qkproj in the project root directory
        static Ref<Project> LoadProject(const std::filesystem::path& projFilePath);

        // takes in path of the **.qkproj in the project root directory
        static Ref<Project> LoadProject(const std::string& title, const std::filesystem::path& projRootDir);

        static inline Ref<Project> LoadProject(const ProjectMetadata& projMeta) {
            return LoadProject(projMeta.Title, projMeta.ProjectRootDirectory);
        }

        static inline bool HaveRecentProjects() noexcept { return !s_RecentProjectsList.empty(); }

        // === Begin: API related to active project ===

        static inline void UnloadActive() noexcept { s_ActiveProject = nullptr; }
        static inline auto GetActive()    noexcept { return s_ActiveProject;    }

        static inline bool SaveActive(const std::filesystem::path& projDirectory) {
            return s_ActiveProject->Save(projDirectory);
        }

        // === End:   API related to active project ===

    private:
        static void CreateProjectDirectoryStructure(const std::filesystem::path& projFilePath, const ProjectConfig& projConfig);

        static void AddRecentProject(ProjectMetadata&& projMeta);

    private:
        static Ref<Project> s_ActiveProject;
        static std::vector<ProjectMetadata> s_RecentProjectsList;
    };

}
