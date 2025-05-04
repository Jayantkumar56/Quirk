
#include "QkEditorpch.h"

#include "ProjectManager.h"

#include "Core/AssetManager/EditorAssetManager.h"


namespace QuirkEditor {

    Quirk::Ref<Quirk::Project> ProjectManager::s_ActiveProject;
    std::vector<ProjectMetadata> ProjectManager::s_RecentProjectsList;

    Quirk::Ref<Quirk::Project> ProjectManager::CreateInDirectory(std::string&& title, const std::filesystem::path& projDirectory) {
        ProjectMetadata projMeta{
            .Title                { std::move(title)               },
            .ProjectRootDirectory { projDirectory / projMeta.Title }
        };

        Quirk::ProjectConfig projConfig = Quirk::ProjectConfig::GetDefaultConfig(projMeta.Title);

        CreateProjectDirectoryStructure(projMeta.ProjectRootDirectory, projConfig);

        s_ActiveProject = Quirk::Project::Create(projMeta.ProjectRootDirectory, std::move(projConfig));
        if (s_ActiveProject == nullptr) {
            return nullptr;
        }

        // saving the .qkproj file
        {
            std::string projFile;

            // setting projfile name
            {
                std::string_view extension = Quirk::Project::GetProjFileExtenstion();
                projFile.reserve(projMeta.Title.size() + extension.size());
                projFile += projMeta.Title;
                projFile += extension;
            }

            std::filesystem::path projFilePath = projMeta.ProjectRootDirectory / projFile;
            Quirk::ProjectSerializer::Serialize(s_ActiveProject, projFilePath);
        }

        AddRecentProject(std::move(projMeta));

        return s_ActiveProject;
    }

    Quirk::Ref<Quirk::Project> ProjectManager::LoadProject(const std::filesystem::path& projFilePath) {
        if (!std::filesystem::is_regular_file(projFilePath)) {
            QK_WARN("Project file is not valid: {0}", projFilePath.string());
            return nullptr;
        }

        s_ActiveProject = Quirk::Project::Load(projFilePath);

        if (s_ActiveProject == nullptr) {
            QK_WARN("Unable to load project at {0}", projFilePath.string());
            return nullptr;
        }

        std::string title = s_ActiveProject->GetTitle();

        AddRecentProject(ProjectMetadata{
            .Title                { std::move(title)                },
            .ProjectRootDirectory { s_ActiveProject->GetDirectory() }
        });

        return s_ActiveProject;
    }

    Quirk::Ref<Quirk::Project> ProjectManager::LoadProject(const std::string& title, const std::filesystem::path& projRootDir) {
        std::string projFile;

        // setting projfile name
        {
            std::string_view extension = Quirk::Project::GetProjFileExtenstion();
            projFile.reserve(title.size() + extension.size());
            projFile += title;
            projFile += extension;
        }

        std::filesystem::path projFilePath = projRootDir / projFile;

        return LoadProject(projFilePath);
    }

    void ProjectManager::CreateProjectDirectoryStructure(const std::filesystem::path& projRootDir, const Quirk::ProjectConfig& projConfig) {
        // main project directory created
        std::filesystem::create_directory(projRootDir);

        // all subdirectories are created
        std::filesystem::create_directory(projRootDir / projConfig.AssetDirectory);
        std::filesystem::create_directory(projRootDir / projConfig.SceneDirectory);
        std::filesystem::create_directory(projRootDir / projConfig.ScriptModulePath);

        // creating the AssetRegistry file
        std::ofstream file(projRootDir / projConfig.AssetRegistryPath);
    }

    void ProjectManager::AddRecentProject(ProjectMetadata&& projMeta) {
        // check if project already exists in the recent list then return
        for (size_t i = 0; i < s_RecentProjectsList.size(); ++i) {
            if (s_RecentProjectsList[i].ProjectRootDirectory == projMeta.ProjectRootDirectory) {
                // making the currently selected item to be the first in the list
                std::rotate(s_RecentProjectsList.begin(), s_RecentProjectsList.begin() + i, s_RecentProjectsList.begin() + i + 1);
                return;
            }
        }

        // not present in the recent list so add to the list in front
        s_RecentProjectsList.emplace(s_RecentProjectsList.begin(), std::move(projMeta.Title), std::move(projMeta.ProjectRootDirectory));
    }

}
