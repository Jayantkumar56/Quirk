

#pragma once

#include "Core/AssetManager/EditorAssetManager.h"
#include "Core/Scene/EditorSceneManager.h"
#include "Core/Project/EditorProject.h"

namespace Quirk {

    // ------------------- Build Configs --------------------------------------------------------------//

    // TODO:
    // 
    // - switch to Runtime versions for Runtime maybe using macros at build time
    // - try to check the requrements met or not, maybe with concepts
    //

    using ProjectConfig = EditorProjectConfig;
    using AssetManager  = EditorAssetManager;
    using SceneManager  = EditorSceneManager;
    using Project       = EditorProject;

}
