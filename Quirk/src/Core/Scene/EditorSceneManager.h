

#pragma once

#include "Core/Core.h"
#include "Core/Scene/Scene.h"
#include "Core/Scene/SceneSerializer.h"

#include <string>
#include <filesystem>


namespace Quirk {

    class EditorSceneManager {
    public:
        EditorSceneManager() noexcept = default;
        EditorSceneManager(std::string activeSceneName, std::unordered_map<std::string, std::filesystem::path> scenesRegistry);

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

    private:
        std::string m_ActiveSceneName;
        Ref<Scene>  m_ActiveScene;

        std::unordered_map<std::string, std::filesystem::path> m_SceneRegistry;
        std::unordered_map<std::string, Ref<Scene>>            m_LoadedScenes;
    };

}
