
#include "Qkpch.h"

#include "EditorSceneManager.h"


namespace Quirk {

    EditorSceneManager::EditorSceneManager(std::string activeSceneName, std::unordered_map<std::string, std::filesystem::path> scenesRegistry) :
        m_ActiveSceneName ( std::move(activeSceneName)                ),
        m_ActiveScene     ( CreateRef<Scene>(m_ActiveSceneName, 0, 0) ),
        m_SceneRegistry   ( std::move(scenesRegistry)                 )
    {
        // loading the active scene
        {
            auto itr = m_SceneRegistry.find(m_ActiveSceneName);

            if (itr != m_SceneRegistry.end()) {
                SceneSerializer::Deserialize(m_ActiveScene, itr->second);
            }
            else {
                QK_CORE_ERROR("Invalid SceneName {0} for Last Active Scene Provided!", m_ActiveSceneName);
            }
        }
    }

    Ref<Scene> EditorSceneManager::CreateEmptyScene(const std::string& sceneName, const std::filesystem::path& scenePath) {
        if (m_SceneRegistry.contains(sceneName)) {
            QK_CORE_WARN("Scene with name \"{0}\" already exists", sceneName);

            // if already loaded then setting the active scene and returning it
            {
                auto itr = m_LoadedScenes.find(sceneName);

                if (itr != m_LoadedScenes.end()) {
                    SetActiveScene(sceneName, itr->second);
                    return m_ActiveScene;
                }
            }

            // loading the scene and setting it as the active scene
            {
                SetActiveScene(sceneName, CreateRef<Scene>(sceneName, 0, 0));
                SceneSerializer::Deserialize(m_ActiveScene, m_SceneRegistry.at(sceneName));
                m_LoadedScenes.emplace(sceneName, m_ActiveScene);
            }

            return m_ActiveScene;
        }

        SetActiveScene(sceneName, CreateRef<Scene>(sceneName, 0, 0));
        m_LoadedScenes.emplace(sceneName, m_ActiveScene);
        m_SceneRegistry.emplace(sceneName, scenePath);

        return m_ActiveScene;
    }

    Ref<Scene> EditorSceneManager::LoadScene(const std::string& sceneName) {
        // if already loaded then setting the active scene and returning it
        {
            auto itr = m_LoadedScenes.find(sceneName);

            if (itr != m_LoadedScenes.end()) {
                SetActiveScene(sceneName, itr->second);
                return m_ActiveScene;
            }
        }

        // loading the scene and setting it as the active scene
        {
            auto itr = m_SceneRegistry.find(sceneName);

            if (itr == m_SceneRegistry.end()) {
                QK_CORE_ERROR("Invalid SceneName {0} Provided!", sceneName);
                return m_ActiveScene;
            }

            SetActiveScene(sceneName, std::move(CreateRef<Scene>(sceneName, 0, 0)));
            SceneSerializer::Deserialize(m_ActiveScene, itr->second);
            m_LoadedScenes.emplace(sceneName, m_ActiveScene);
        }

        return m_ActiveScene;
    }

}
