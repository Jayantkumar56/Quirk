

#pragma once

#include "Core/Core.h"
#include "Core/Config.h"
#include "ProjectConfig.h"
#include "Core/Project/ProjectSerializer.h"
#include "Core/Scene/EditorSceneManager.h"
#include "Core/AssetManager/EditorAssetManager.h"

#include <string>
#include <filesystem>

namespace Quirk {

    class EditorProject {
    private:
        // Private constructor to prevent direct instantiation. Use Create() or Load() instead.
        inline EditorProject(std::filesystem::path&& projRootDir, ProjectConfig&& config) noexcept :
            m_Config(std::move(config)),
            m_ProjectRootDirectory(std::move(projRootDir))
        {
        }

    public:
        static inline Ref<EditorProject> Create(std::filesystem::path projRootDir, ProjectConfig&& config) {
            return Ref<EditorProject>(new EditorProject(std::move(projRootDir), std::move(config)));
        }

        static inline Ref<EditorProject> Load(const std::filesystem::path& projFilePath) {
            ProjectConfig config;
            if (!EditorProjectSerializer::DeserializeConfig(config, projFilePath)) {
                QK_WARN("Unable to Deserialize proj file!");
                return nullptr;
            }

            return Ref<EditorProject>(new EditorProject(std::move(projFilePath.parent_path()), std::move(config)));
        }

        static inline std::string_view GetProjFileExtenstion() noexcept { return ".qkproj"; }

    public:
        inline bool Save(const std::filesystem::path& projDirectory) {
            std::filesystem::path projRootDir = projDirectory / m_Config.Name;
            std::string projFile;

            // setting projfile name
            {
                std::string_view extension = EditorProject::GetProjFileExtenstion();
                projFile.reserve(m_Config.Name.size() + extension.size());
                projFile += m_Config.Name;
                projFile += extension;
            }

            if (EditorProjectSerializer::Serialize(Ref<EditorProject>(this), projRootDir / projFile)) {
                m_ProjectRootDirectory = projRootDir;
                return true;
            }

            return false;
        }

        inline const auto& GetTitle()        const noexcept { return m_Config.Name; }
        inline const auto& GetConfig()       const noexcept { return m_Config; }
        inline const auto& GetDirectory()    const noexcept { return m_ProjectRootDirectory; }
        inline const auto& GetAssetManager() const noexcept { return m_AssetManager; }

        inline auto GetAssetDirectory() const noexcept {
            return m_ProjectRootDirectory / m_Config.AssetDirectory;
        }

        inline auto GetAssetFileSystemPath(const std::filesystem::path& path) const noexcept {
            return GetAssetDirectory() / path;
        }

        inline void SetRootDirectory(const std::filesystem::path& projRoot) noexcept {
            m_ProjectRootDirectory = projRoot;
        }

    private:
        ProjectConfig m_Config;
        AssetManager  m_AssetManager;
        SceneManager  m_SceneManager;
        std::filesystem::path m_ProjectRootDirectory;
    };

}
