

#pragma once

#include "Core/Core.h"
#include "EditorProjectConfig.h"
#include "Core/AssetManager/EditorAssetManager.h"
#include "Core/Scene/EditorSceneManager.h"

#include <filesystem>

namespace Quirk {

    class EditorProject {
        friend class ProjectManager;

    public:
        inline EditorProject(EditorProjectConfig config, EditorAssetManager assetManager, EditorSceneManager sceneManager) noexcept :
                m_Config       ( std::move(config)       ),
                m_AssetManager ( std::move(assetManager) ),
                m_SceneManager ( std::move(sceneManager) )
        {
            // NOTE:
            // 
            // - m_ProjectRootDirectory must be initialized by the Project manager (or the creator of this object)
            //   use SetProjectRootDirectory to initialize it. This function is private to prevent unrelated code from accessing it
            //   thus the creator must be friend of this class
        }

        inline EditorProject(EditorProjectConfig config) noexcept :
                m_Config ( std::move(config) )
        {
            // NOTE:
            // 
            // - m_ProjectRootDirectory must be initialized by the Project manager (or the creator of this object)
            //   use SetProjectRootDirectory to initialize it. This function is private to prevent unrelated code from accessing it
            //   thus the creator must be friend of this class
        }

        inline const auto& GetTitle()        const noexcept { return m_Config.ProjectName;   }
        inline const auto& GetConfig()       const noexcept { return m_Config;               }
        inline const auto& GetDirectory()    const noexcept { return m_ProjectRootDirectory; }
        inline const auto& GetAssetManager() const noexcept { return m_AssetManager;         }
        inline const auto& GetSceneManager() const noexcept { return m_SceneManager;         }

        inline auto GetAssetDirectory() const noexcept {
            return m_ProjectRootDirectory / m_Config.AssetDirectory;
        }

        inline auto GetAssetFileSystemPath(const std::filesystem::path& path) const noexcept {
            return GetAssetDirectory() / path;
        }

        inline void SetRootDirectory(const std::filesystem::path& projRoot) noexcept {
            m_ProjectRootDirectory = projRoot;
        }

    public:
        static inline std::string_view GetProjFileExtenstion() noexcept { return ".qkproj"; }

    private:
        inline void SetProjectRootDirectory(std::filesystem::path directory) noexcept { 
            m_ProjectRootDirectory = std::move(directory); 
        }

    private:
        EditorProjectConfig   m_Config;
        std::filesystem::path m_ProjectRootDirectory;

        EditorAssetManager    m_AssetManager;
        EditorSceneManager    m_SceneManager;
    };

}
