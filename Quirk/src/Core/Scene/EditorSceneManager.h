

#pragma once

#include "Core/Core.h"
#include "Core/Scene/Scene.h"

#include <string>
#include <filesystem>


namespace Quirk {

    struct SceneMeta {
        std::string Name;
        std::filesystem::path Path;
    };

    class EditorSceneManager {
    public:


    private:
        std::string m_ActiveSceneName;
        Ref<Scene>  m_ActiveScene;

        std::unordered_map<std::string, Ref<Scene>> m_LoadedScenes;

        std::unordered_map<UUID, SceneMeta> m_ScenesRegistry;
    };

}
