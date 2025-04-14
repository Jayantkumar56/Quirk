

#pragma once

#include "Core/Project/Project.h"

namespace Quirk {

    class ProjectManager {
    public:
        // the path of projMetaData should be correct and according to the ProjectMetadata path specification rule
        static Ref<Project> CreateNewProject(ProjectMetadata& projMetaData);

        static inline const std::vector<ProjectMetadata>& GetRecentProjectsList() noexcept { return s_RecentProjectsList; }

        // takes path of the folder in which project file (**.qkproj) lives
        // return file path of the project file (**.qkproj) if successful
        // in case of error or not finding project file it return empty path
        static std::filesystem::path AddRecentProject(const std::filesystem::path& directory);

        // takes path of the yaml file which contains the recent projects list
        static inline bool LoadRecentProjectsList(const std::filesystem::path& filePath) {
            return ProjectSerializer::DeserializeRecentProjectsList(s_RecentProjectsList, filePath);
        }

        // takes path of the file where recent list should be stored
        static inline bool SaveRecentProjectsList(const std::filesystem::path& filePath) {
            return ProjectSerializer::SerializeRecentProjectsList(s_RecentProjectsList, filePath);
        }

    private:
        static std::vector<ProjectMetadata> s_RecentProjectsList;
    };

}
