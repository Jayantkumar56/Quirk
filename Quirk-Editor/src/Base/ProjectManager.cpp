
#include "QkEditorpch.h"

#include "ProjectManager.h"

#include "Core/AssetManager/EditorAssetManager.h"
#include "Core/Reflection/Registrations/EditorProjectReflection.h"
#include "Core/Serialization/Serialization.h"


namespace QuirkEditor {

    constexpr bool val = std::is_same_v<Quirk::Reflect<Quirk::Project>::Create, Quirk::Reflect<Quirk::Project>::CreateOrVoid_T>;


    Quirk::Ref<Quirk::Project> ProjectManager::s_ActiveProject;
    std::vector<ProjectMetadata> ProjectManager::s_RecentProjectsList;

    Quirk::Ref<Quirk::Project> ProjectManager::CreateInDirectory(std::string&& title, const std::filesystem::path& projDirectory) {
        ProjectMetadata projMeta{
            .Title                { std::move(title)               },
            .ProjectRootDirectory { projDirectory / projMeta.Title }
        };

        Quirk::ProjectConfig projConfig = Quirk::ProjectConfig::GetDefaultConfig(projMeta.Title);

        CreateProjectDirectoryStructure(projMeta.ProjectRootDirectory, projConfig);

        s_ActiveProject = Quirk::CreateRef<Quirk::Project>(std::move(projConfig));
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

            try {
                Quirk::Serialization::Serialize(s_ActiveProject, projFilePath);
            }
            catch (const std::exception& e) {
                QK_CORE_ERROR("Serialization of Active Project failed with error: {0}", e.what());
            }
        }

        AddRecentProject(std::move(projMeta));

        return s_ActiveProject;
    }

    Quirk::Ref<Quirk::Project> ProjectManager::LoadProject(const std::filesystem::path& projFilePath) {
        if (!std::filesystem::is_regular_file(projFilePath)) {
            QK_WARN("Project file is not valid: {0}", projFilePath.string());
            return nullptr;
        }

        try {
            s_ActiveProject = Quirk::Serialization::Deserialize<Quirk::Ref<Quirk::Project>>(projFilePath);
            s_ActiveProject->SetProjectRootDirectory(projFilePath.parent_path());

            std::string title = s_ActiveProject->GetTitle();

            AddRecentProject(ProjectMetadata{
                .Title                { std::move(title)                },
                .ProjectRootDirectory { s_ActiveProject->GetDirectory() }
            });

            return s_ActiveProject;
        }
        catch (const std::exception& e) {
            QK_CORE_ERROR("Deserialization of Project with path {0} failed with error: {1}", projFilePath.string(), e.what());
            s_ActiveProject = nullptr;
            return nullptr;
        }
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

    void ProjectManager::AddRecentProject(ProjectMetadata projMeta) {
        // check if project already exists in the recent list then return
        for (auto it = s_RecentProjectsList.begin(); it != s_RecentProjectsList.end(); ++it) {
            if (it->ProjectRootDirectory == projMeta.ProjectRootDirectory) {
                // making the currently selected item to be the first in the list
                std::rotate(s_RecentProjectsList.begin(), it, it + 1);
                return;
            }
        }

        // not present in the recent list so add to the list in front
        s_RecentProjectsList.emplace(s_RecentProjectsList.begin(), std::move(projMeta));
    }

}
