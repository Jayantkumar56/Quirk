

#include "QkEditorpch.h"

#include "ProjectManager.h"
#include "Base/Reflection/ProjectManagerReflection.h"

#include "Core/AssetManager/EditorAssetManager.h"
#include "Core/Serialization/Serialization.h"



namespace QuirkEditor {

    void ProjectManager::LoadRecentProjectsList(const std::filesystem::path& projListfilePath) noexcept {
        try {
            m_RecentProjectsList = Quirk::Serialization::DeserializeWithFieldName<std::vector<ProjectMetadata>>(
                "Recent Projects",
                projListfilePath
            );
        }
        catch (const std::exception& e) {
            QK_ERROR("Loading RecentProjectsList with path {0} failed with error: {1}", projListfilePath.string(), e.what());
        }
        catch (...) {
            QK_ERROR("Loading RecentProjectsList with path {0} failed with some unknown error.", projListfilePath.string());
        }
    }

    void ProjectManager::SaveRecentProjectsList(const std::filesystem::path& projListfilePath) const {
        try {
            if (!Quirk::Serialization::SerializeWithFieldName("Recent Projects", m_RecentProjectsList, projListfilePath)) {
                QK_ERROR("Saving RecentProjectsList with path {0} failed.", projListfilePath.string());
            }
        }
        catch (...) {
            QK_ERROR("Saving RecentProjectsList with path {0} failed with some unknown error.", projListfilePath.string());
        }
    }

    Quirk::Ref<Quirk::Project> ProjectManager::CreateInDirectory(std::string&& title, const std::filesystem::path& projDirectory) {
        ProjectMetadata projMeta{
            .Title                { std::move(title)               },
            .ProjectRootDirectory { projDirectory / projMeta.Title }
        };

        Quirk::ProjectConfig projConfig = Quirk::ProjectConfig::GetDefaultConfig(projMeta.Title);

        CreateProjectDirectoryStructure(projMeta.ProjectRootDirectory, projConfig);

        m_ActiveProject = Quirk::CreateRef<Quirk::Project>(std::move(projConfig));
        if (m_ActiveProject == nullptr) {
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
                Quirk::Serialization::Serialize(m_ActiveProject, projFilePath);
            }
            catch (const std::exception& e) {
                QK_ERROR("Serialization of Active Project failed with error: {0}", e.what());
            }
            catch (...) {
                QK_ERROR("Serialization of Active Project failed with some unknown error.");
            }
        }

        AddRecentProject(std::move(projMeta));

        return m_ActiveProject;
    }

    Quirk::Ref<Quirk::Project> ProjectManager::LoadProject(const std::filesystem::path& projFilePath) {
        if (!std::filesystem::is_regular_file(projFilePath)) {
            QK_WARN("Project file is not valid: {0}", projFilePath.string());
            return nullptr;
        }

        try {
            m_ActiveProject = Quirk::Serialization::Deserialize<Quirk::Ref<Quirk::Project>>(projFilePath);
            m_ActiveProject->Init(projFilePath.parent_path());

            std::string title = m_ActiveProject->GetTitle();

            AddRecentProject(ProjectMetadata{
                .Title                { std::move(title)                },
                .ProjectRootDirectory { m_ActiveProject->GetDirectory() }
            });

            return m_ActiveProject;
        }
        catch (const std::exception& e) {
            QK_ERROR("Deserialization of Project with path {0} failed with error: {1}", projFilePath.string(), e.what());
            m_ActiveProject = nullptr;
            return nullptr;
        }
        catch (...) {
            QK_ERROR("Deserialization of Active Project failed with some unknown error.");
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
        for (auto it = m_RecentProjectsList.begin(); it != m_RecentProjectsList.end(); ++it) {
            if (it->ProjectRootDirectory == projMeta.ProjectRootDirectory) {
                // making the currently selected item to be the first in the list
                std::rotate(m_RecentProjectsList.begin(), it, it + 1);
                return;
            }
        }

        // not present in the recent list so add to the list in front
        m_RecentProjectsList.emplace(m_RecentProjectsList.begin(), std::move(projMeta));
    }

}
