

#pragma once

#include "Core/Core.h"
#include "EditorProjectConfig.h"
#include "Core/AssetManager/EditorAssetManager.h"
#include "Core/Scene/EditorSceneManager.h"

#include <filesystem>

namespace Quirk {

    // NOTE:
    // 
    // - Init must be called after construction EditorProject object

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

        inline void Init(std::filesystem::path directory) noexcept {
            m_ProjectRootDirectory = std::move(directory);
            m_SceneManager.Init(m_ProjectRootDirectory, &m_AssetManager);
        }

        inline const auto& GetTitle()        const noexcept { return m_Config.ProjectName;   }
        inline const auto& GetConfig()       const noexcept { return m_Config;               }
        inline const auto& GetDirectory()    const noexcept { return m_ProjectRootDirectory; }
        inline const auto& GetAssetManager() const noexcept { return m_AssetManager;         }
        inline const auto& GetSceneManager() const noexcept { return m_SceneManager;         }

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
