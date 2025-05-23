


#pragma once

#include "Base/ProjectManager.h"

#include "Core/Reflection/Reflection.h"
#include "Core/Application/EditorProjectConfig.h"
#include "Core/AssetManager/EditorAssetManager.h"
#include "Core/Scene/EditorSceneManager.h"
#include "Core/Application/EditorProject.h"


namespace Quirk {

    REGISTER_REFLECTION(QuirkEditor::ProjectMetadata, "Project Meta", (CONSTRUCTOR),
        ( Title,         Title,                 DUMMY_SETTER, PROPFLAG_DIRECT_MEMBER | PROPFLAG_SERIALIZABLE ),
        ( RootDirectory, ProjectRootDirectory,  DUMMY_SETTER, PROPFLAG_DIRECT_MEMBER | PROPFLAG_SERIALIZABLE )
    );
    
    //=============================================================================================================================
    //--------- Editor Project Related Reflection Registrations -------------------------------------------------------------------

    REGISTER_REFLECTION(EditorProject, "Project", (CONSTRUCTOR),
        ( ProjectConfigs, GetConfig,       DUMMY_SETTER, PROPFLAG_SERIALIZABLE ),
        ( RootDirectory,  GetDirectory,    DUMMY_SETTER, PROPFLAG_NONE         ),
        ( AssetManager,   GetAssetManager, DUMMY_SETTER, PROPFLAG_SERIALIZABLE ),
        ( SceneManager,   GetSceneManager, DUMMY_SETTER, PROPFLAG_SERIALIZABLE )
    );


    REGISTER_REFLECTION(EditorProjectConfig, "ProjectConfigs", (CONSTRUCTOR),
        ( ProjectName,       ProjectName,       DUMMY_SETTER, PROPFLAG_DIRECT_MEMBER | PROPFLAG_SERIALIZABLE ),
        ( StartScene,        StartScene,        DUMMY_SETTER, PROPFLAG_DIRECT_MEMBER | PROPFLAG_SERIALIZABLE ),
        ( AssetDirectory,    AssetDirectory,    DUMMY_SETTER, PROPFLAG_DIRECT_MEMBER | PROPFLAG_SERIALIZABLE ),
        ( SceneDirectory,    SceneDirectory,    DUMMY_SETTER, PROPFLAG_DIRECT_MEMBER | PROPFLAG_SERIALIZABLE ),
        ( ScriptModulePath,  ScriptModulePath,  DUMMY_SETTER, PROPFLAG_DIRECT_MEMBER | PROPFLAG_SERIALIZABLE ),
        ( AssetRegistryPath, AssetRegistryPath, DUMMY_SETTER, PROPFLAG_DIRECT_MEMBER | PROPFLAG_SERIALIZABLE )
    );


    REGISTER_REFLECTION(EditorAssetManager, "AssetManager", (CONSTRUCTOR),
        ( AssetRegistry, GetAssetRegistry, DUMMY_SETTER, PROPFLAG_SERIALIZABLE)
    );


    REGISTER_REFLECTION(EditorSceneManager, "SceneManager", (CONSTRUCTOR),
        ( ActiveSceneName, GetActiveSceneName, DUMMY_SETTER, PROPFLAG_SERIALIZABLE ),
        ( ActiveScene,     GetActiveScene,     DUMMY_SETTER, PROPFLAG_NONE         ),
        ( SceneRegistry,   GetSceneRegistry,   DUMMY_SETTER, PROPFLAG_SERIALIZABLE ),
        ( LoadedScenes,    GetLoadedScenes,    DUMMY_SETTER, PROPFLAG_NONE         )
    );

    //_____________________________________________________________________________________________________________________________

}
