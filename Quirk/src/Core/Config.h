

#pragma once

namespace Quirk {

    // ------------------- Build Configs --------------------------------------------------------------//

    // TODO:
    // 
    // - switch to Runtime versions for Runtime maybe using macros at build time
    // - try to check the requrements met or not, maybe with concepts
    //

    class EditorAssetManager;
    class EditorSceneManager;
    class EditorProject;

    using AssetManager = EditorAssetManager;
    using SceneManager = EditorSceneManager;
    using Project      = EditorProject;

}
