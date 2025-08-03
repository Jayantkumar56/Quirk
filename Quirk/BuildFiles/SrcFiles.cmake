

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

# Source files in Core/Application
RegisterSourceFiles(QUIRK_SOURCE_FILES "${BASE_SOURCE_DIRECTORY}/Core/Application"
    "Application.h"
    "Application.cpp"
    "EditorProject.h"
    "EditorProjectConfig.h"
    "EntryPoint.h"
)

# Source files in Core/AssetManager
RegisterSourceFiles(QUIRK_SOURCE_FILES "${BASE_SOURCE_DIRECTORY}/Core/AssetManager"
    "AssetStorage.h"
    "AssetManager.h"
    "EditorAssetManager.h"
)

# Source files in Core/AssetImporter
RegisterSourceFiles(QUIRK_SOURCE_FILES "${BASE_SOURCE_DIRECTORY}/Core/AssetImporter"
    "AssetImporter.h"
    "TextureImporter.h"
    "TextureImporter.cpp"
)

# Source files in Core/Assets
RegisterSourceFiles(QUIRK_SOURCE_FILES "${BASE_SOURCE_DIRECTORY}/Core/Assets"
    "Geometry/Geometry.h"
    "Geometry/Geometry.cpp"
    "Geometry/PrimitiveMeshGenerator.h"
    "Geometry/PrimitiveMeshGenerator.cpp"
    "RenderAssets/ShaderLibrary.h"
    "RenderAssets/ShaderLibrary.cpp"
    "AssetHandle.h"
    "TextureAsset.h"
)

# Source files in Core/Camera
RegisterSourceFiles(QUIRK_SOURCE_FILES "${BASE_SOURCE_DIRECTORY}/Core/Camera"
    "Camera.h"
    "CameraController.h"
    "CameraController.cpp"
)

# Source files in Core/Frame
RegisterSourceFiles(QUIRK_SOURCE_FILES "${BASE_SOURCE_DIRECTORY}/Core/Frame"
    "FontManager.h"
    "Frame.h"
    "FrameBase.h"
    "FrameInitContext.h"
    "FrameManager.h"
    "FrameTraits.h"
    "GraphicalContextManager.h"
    "ImguiBuild.cpp"
    "ImguiContextManager.h"
    "ImguiUI.h"
    "ImguiUI.cpp"
    "Panel.h"
    "PanelManager.h"
    "TitleBar.h"
    "TitleBarManager.h"
    "Window.h"
    "WindowManager.h"
)

# Source files in Core/Geometry
RegisterSourceFiles(QUIRK_SOURCE_FILES "${BASE_SOURCE_DIRECTORY}/Core/Geometry"
    "Mesh.h"
)

# Source files in Core/Input
RegisterSourceFiles(QUIRK_SOURCE_FILES "${BASE_SOURCE_DIRECTORY}/Core/Input"
    "ApplicationEvents.h"
    "Events.h"
    "Input.h"
    "Input.cpp"
    "KeyboardEvents.h"
    "KeyCodes.h"
    "MouseEvents.h"
)

# Source files in Core/Reflection
RegisterSourceFiles(QUIRK_SOURCE_FILES "${BASE_SOURCE_DIRECTORY}/Core/Reflection"
    "Registrations/ComponentList.h"
    "Registrations/TextureAsset.h"
    "Registrations/TextureEnums.h"
    "CommonMacros.h"
    "EnumRegistry.h"
    "Reflection.h"
)

# Source files in Core/Renderer
RegisterSourceFiles(QUIRK_SOURCE_FILES "${BASE_SOURCE_DIRECTORY}/Core/Renderer"
    "Light/Light.h"
    "Light/Light.cpp"
    "EditorSceneRenderer.h"
    "EditorSceneRenderer.cpp"
    "Renderer.h"
    "Renderer.cpp"
)

# Source files in Core/RHI
RegisterSourceFiles(QUIRK_SOURCE_FILES "${BASE_SOURCE_DIRECTORY}/Core/RHI"
    "Factory.h"
    "FrameBuffer.h"
    "GraphicalContext.h"
    "GraphicsAPI.h"
    "IndexBuffer.h"
    "RenderCommand.h"
    "RenderSystem.h"
    "RenderSystem.cpp"
    "Shader.h"
    "Texture2D.h"
    "VertexArray.h"
    "VertexBuffer.h"
)

# Source files in Core/Scene
RegisterSourceFiles(QUIRK_SOURCE_FILES "${BASE_SOURCE_DIRECTORY}/Core/Scene"
    "Components.h"
    "EditorSceneManager.h"
    "EditorSceneManager.cpp"
    "Entity.h"
    "Scene.h"
    "Scene.cpp"
    "SceneCamera.h"
    "SceneCamera.cpp"
    "SceneSerializer.h"
    "SceneSerializer.cpp"
    "ScriptableEntity.h"
)

# Source files in Core/Serialization
RegisterSourceFiles(QUIRK_SOURCE_FILES "${BASE_SOURCE_DIRECTORY}/Core/Serialization"
    "ContainerDeserializer.h"
    "ContainerSerializer.h"
    "Deserializer.h"
    "PrimitiveDeserializer.h"
    "PrimitiveSerializer.h"
    "Serialization.h"
    "Serializer.h"
)

# Source files in Core/Utility
RegisterSourceFiles(QUIRK_SOURCE_FILES "${BASE_SOURCE_DIRECTORY}/Core/Utility"
    "Buffer.h"
    "DialogBox.h"
    "ImguiUIUtility.h"
    "ImguiUIUtility.cpp"
    "SmartPointer.h"
    "Time.h"
    "TypeTraits.h"
    "UUID.h"
    "UUID.cpp"
    "View.h"
    "YamlUtility.h"
)

# Source files in Core
RegisterSourceFiles(QUIRK_SOURCE_FILES "${BASE_SOURCE_DIRECTORY}/Core"
    "Config.h"
    "Core.h"
)


# Source files in Debug
RegisterSourceFiles(QUIRK_SOURCE_FILES "${BASE_SOURCE_DIRECTORY}/Debug"
    "Log.h"
    "Log.cpp"
)


# Source files in Platform/OpenGL
RegisterSourceFiles(QUIRK_SOURCE_FILES "${BASE_SOURCE_DIRECTORY}/Platform/OpenGL"
    "OpenGLFactory.h"
    "OpenGLFrameBuffer.h"
    "OpenGLFrameBuffer.cpp"
    "OpenGLGraphicalContext.h"
    "OpenGLIndexBuffer.h"
    "OpenGLIndexBuffer.cpp"
    "OpenGLRenderCommand.h"
    "OpenGLRenderCommand.cpp"
    "OpenGLShader.h"
    "OpenGLShader.cpp"
    "OpenGLTexture2D.h"
    "OpenGLTexture2D.cpp"
    "OpenGLVertexArray.h"
    "OpenGLVertexArray.cpp"
    "OpenGLVertexBuffer.h"
    "OpenGLVertexBuffer.cpp"
    "WGLExtensions.h"
    "WindowsOpenGLContext.h"
    "WindowsOpenGLContext.cpp"
)

# Source files in Platform/OpenGL
RegisterSourceFiles(QUIRK_SOURCE_FILES "${BASE_SOURCE_DIRECTORY}/Platform/Windows"
    "WindowsDialogBox.h"
    "WindowsDialogBox.cpp"
    "WindowsInput.cpp"
    "WindowsTime.h"
    "WindowsTime.cpp"
    "WindowsWindow.h"
    "WindowsWindow.cpp"
)


# Source files in src
RegisterSourceFiles(QUIRK_SOURCE_FILES "${BASE_SOURCE_DIRECTORY}"
    "Qkpch.h"
    "Quirk.h"
)

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${QUIRK_SOURCE_FILES})
