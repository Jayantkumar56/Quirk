

#pragma once

#include "Core/Core.h"
#include "Core/Scene/SceneSerializer.h"
#include "Core/Scene/Scene.h"

#include <string>
#include <filesystem>


namespace Quirk {

    class EditorSceneManager {
    public:
        EditorSceneManager() noexcept = default;
        EditorSceneManager(std::string activeSceneName, std::unordered_map<std::string, std::filesystem::path> scenesRegistry) noexcept:
                m_ActiveSceneName ( std::move(activeSceneName)                ),
                m_ActiveScene     ( CreateRef<Scene>(m_ActiveSceneName, 0, 0) ),
                m_SceneRegistry   ( std::move(scenesRegistry)                 )
        {}

        void Init(std::filesystem::path projRootDirectory, View<EditorAssetManager> assetManager) noexcept;

        Ref<Scene> CreateEmptyScene(const std::string& sceneName, const std::filesystem::path& scenePath);
        Ref<Scene> LoadScene(const std::string& sceneName);

        inline void SaveActive() {
            SceneSerializer::Serialize(m_ActiveScene, m_SceneRegistry.at(m_ActiveSceneName));
        }

        inline void SetActiveScene(std::string name, Ref<Scene> scene) noexcept {
            m_ActiveSceneName = std::move( name  );
            m_ActiveScene     = std::move( scene );
        }

        inline auto&              GetActiveSceneRefView()    noexcept { return m_ActiveScene;     }
        inline       Ref<Scene>   GetActiveScene()           noexcept { return m_ActiveScene;     }
        inline const std::string& GetActiveSceneName() const noexcept { return m_ActiveSceneName; }
        inline const auto&        GetSceneRegistry()   const noexcept { return m_SceneRegistry;   }
        inline const auto&        GetLoadedScenes()    const noexcept { return m_LoadedScenes;    }
 
        inline std::optional<std::filesystem::path> GetScenePath(const std::string& sceneName) const noexcept {
            auto it = m_SceneRegistry.find(sceneName);

            if (it != m_SceneRegistry.end())
                return m_ProjRootDirectory / it->second;

            return std::nullopt;
        }

    private:
        std::string m_ActiveSceneName;
        Ref<Scene>  m_ActiveScene;

        std::unordered_map<std::string, std::filesystem::path> m_SceneRegistry;
        std::unordered_map<std::string, Ref<Scene>>            m_LoadedScenes;

        std::filesystem::path    m_ProjRootDirectory;
        View<EditorAssetManager> m_AssetManager;
    };

}
