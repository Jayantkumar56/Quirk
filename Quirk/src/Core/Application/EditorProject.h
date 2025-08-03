

#pragma once

#include "Core/Core.h"
#include "EditorProjectConfig.h"
#include "Core/AssetManager/EditorAssetManager.h"
#include "Core/Scene/EditorSceneManager.h"
#include "Core/RHI/Factory.h"

#include <filesystem>

namespace Quirk {

    // NOTE:
    // 
    // - m_ProjectRootDirectory field is not set by the constructor, it must be manually assigned
    //
    // - Init() must be called once, and only after the construction of the EditorProject object.
    //   This function performs initialization that cannot be done safely in the constructor.
    //
    // - Typically, the parent system (e.g. EditorFrame or ProjectManager) is responsible for setting
    //   m_ProjectRootDirectory and calling Init() in the correct order.

    class EditorProject {
    public:
        inline EditorProject(EditorProjectConfig config, EditorAssetManager assetManager, EditorSceneManager sceneManager) noexcept :
                m_Config       ( std::move(config)       ),
                m_AssetManager ( std::move(assetManager) ),
                m_SceneManager ( std::move(sceneManager) )
        {}

        inline EditorProject(EditorProjectConfig config) noexcept :
                m_Config ( std::move(config) )
        {}

        inline void SetProjectRootDirectory(std::filesystem::path directory) noexcept {
            QK_ASSERT(m_ProjectRootDirectory.empty(), "Project root directory is already set!");

            m_ProjectRootDirectory = std::move(directory);
            m_SceneManager.Init(m_ProjectRootDirectory, &m_AssetManager);
        }

        inline void SetRHIFactory(ConstView<RHI::Factory> factory) noexcept {
            QK_ASSERT(!m_AssetManager.HasFactory(), "Factory is already set!");
            m_AssetManager.SetRHIFactory(factory);
        }

        inline const auto& GetTitle()               const noexcept { return m_Config.ProjectName;   }
        inline const auto& GetConfig()              const noexcept { return m_Config;               }
        inline const auto& GetDirectory()           const noexcept { return m_ProjectRootDirectory; }
        inline const auto& GetAssetManagerForRead() const noexcept { return m_AssetManager;         }
        inline const auto& GetSceneManagerForRead() const noexcept { return m_SceneManager;         }

        inline auto& GetAssetManager() noexcept { return m_AssetManager; }
        inline auto& GetSceneManager() noexcept { return m_SceneManager; }

        // === Begin: Utility Getters =======

        inline auto& GetActiveSceneRefView() noexcept { 
            return m_SceneManager.GetActiveSceneRefView(); 
        }

        inline auto GetAssetDirectory() const noexcept { 
            return m_ProjectRootDirectory / m_Config.AssetDirectory; 
        }

        inline auto GetAssetFileSystemPath(const std::filesystem::path& path) const noexcept { 
            return GetAssetDirectory() / path; 
        }

        inline auto GetProjectRootDirectory() const { return m_ProjectRootDirectory; }

        inline auto GetProjFilePath() const noexcept {
            std::filesystem::path projFile     = m_Config.ProjectName + ".qkproj";
            std::filesystem::path projFilePath = m_ProjectRootDirectory / projFile;
            return projFilePath;
        }

        // === End:   Utility Getters =======

    public:
        static inline std::string_view GetProjFileExtenstion() noexcept { return ".qkproj"; }

    private:
        EditorProjectConfig   m_Config;
        std::filesystem::path m_ProjectRootDirectory;

        EditorAssetManager    m_AssetManager;
        EditorSceneManager    m_SceneManager;
    };

}
