

#pragma once

namespace QuirkEditor {

    struct ProjectMetadata;

    class ProjectManagerSerializer {
    public:
        static bool SerializeRecentProjectsList(std::vector<ProjectMetadata>& list, const std::filesystem::path& filePath);
        static bool DeserializeRecentProjectsList(std::vector<ProjectMetadata>& list, const std::filesystem::path& filePath);
    };

}

