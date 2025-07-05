
function(RegisterSourceFiles FILE_LIST_VAR PREFIX)
    if(DEFINED ${FILE_LIST_VAR})
        set(RESULT_LIST "${${FILE_LIST_VAR}}")
    else()
        set(RESULT_LIST "")
    endif()

    foreach(FILE_PATH ${ARGN})
        list(APPEND RESULT_LIST "${PREFIX}/${FILE_PATH}")
    endforeach()

    set(${FILE_LIST_VAR} "${RESULT_LIST}" PARENT_SCOPE)
endfunction(RegisterSourceFiles)


set(BASE_SOURCE_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/src")

# Source files in Base
RegisterSourceFiles(QUIRK_EDITOR_SOURCE_FILES "${BASE_SOURCE_DIRECTORY}/Base"
    "Reflection/ProjectManagerReflection.h"
    "ProjectManager.h"
    "ProjectManager.cpp"
    "SelectionContext.h"
)

# Source files in Editor/Panels
RegisterSourceFiles(QUIRK_EDITOR_SOURCE_FILES "${BASE_SOURCE_DIRECTORY}/Editor/Panels"
    "InspectorPanel/EntityInspectorUI.h"
    "InspectorPanel/EntityInspectorUI.cpp"
    "InspectorPanel/InspectorPanel.h"
    "InspectorPanel/InspectorPanel.cpp"
    "ContentBrowserPanel.h"
    "ContentBrowserPanel.cpp"
    "SceneHierarchyPanel.h"
    "SceneHierarchyPanel.cpp"
    "SceneViewportPanel.h"
    "SceneViewportPanel.cpp"
)

# Source files in Editor
RegisterSourceFiles(QUIRK_EDITOR_SOURCE_FILES "${BASE_SOURCE_DIRECTORY}/Editor"
    "EditorCamera.h"
    "EditorCamera.cpp"
    "EditorFrame.h"
    "EditorFrame.cpp"
    "EditorFrameResourceManager.h"
    "EditorTheme.h"
    "EditorTheme.cpp"
    "EditorTitleBar.h"
    "EditorTitleBar.cpp"
)

# Source files in Launcher
RegisterSourceFiles(QUIRK_EDITOR_SOURCE_FILES "${BASE_SOURCE_DIRECTORY}/Launcher"
    "LauncherFrame.h"
    "LauncherFrame.cpp"
    "LauncherTitleBar.h"
    "LauncherTitleBar.cpp"
)

# Source files in src
RegisterSourceFiles(QUIRK_EDITOR_SOURCE_FILES "${BASE_SOURCE_DIRECTORY}"
    "QkEditorpch.h"
    "QuirkEditorApp.h"
    "QuirkEditorApp.cpp"
)


source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${QUIRK_EDITOR_SOURCE_FILES})
