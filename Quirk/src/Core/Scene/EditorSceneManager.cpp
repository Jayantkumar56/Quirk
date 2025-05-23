

#include "Qkpch.h"

#include "EditorSceneManager.h"


namespace Quirk {

    void EditorSceneManager::Init(std::filesystem::path projRootDirectory, EditorAssetManager* assetManager) noexcept {
        m_ProjRootDirectory = std::move(projRootDirectory);
        m_AssetManager      = assetManager;

        // loading the active scene
        {
            if (auto path = GetScenePath(m_ActiveSceneName)) {
                SceneSerializer::Deserialize(m_ActiveScene, path.value());
                m_ActiveScene->Init(m_AssetManager);
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
                m_ActiveScene->Init(m_AssetManager);
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
            m_ActiveScene->Init(m_AssetManager);
            m_LoadedScenes.emplace(sceneName, m_ActiveScene);
        }

        return m_ActiveScene;
    }

}
