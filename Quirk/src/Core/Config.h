

#pragma once


namespace Quirk {

    // ------------------- Build Configs --------------------------------------------------------------//

    // TODO:
    // 
    // - switch to Runtime versions for Runtime maybe using macros at build time
    // - try to check the requrements met or not, maybe with concepts
    //

    // NOTE:
    // 
    // - forward declaring to avoid cyclic dependency
    // - include actual header for concrete types

    struct EditorProjectConfig;
    class  EditorAssetManager;
    class  EditorSceneManager;
    class  EditorProject;

    using ProjectConfig = EditorProjectConfig;
    using AssetManager  = EditorAssetManager;
    using SceneManager  = EditorSceneManager;
    using Project       = EditorProject;

}
