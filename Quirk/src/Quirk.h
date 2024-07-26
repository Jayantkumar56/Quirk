

#pragma once

#include "Core/Core.h"
#include "Debug/Log.h"

// Include all GLM core / GLSL features
#include "glm/glm.hpp" // vec2, vec3, mat4, radians
// Include all GLM extensions
#include "glm/ext.hpp" // perspective, translate, rotate

#include "Core/Camera/OrthographicCamera.h"

#include "Core/Input/KeyCodes.h"
#include "Core/Input/Events.h"
#include "Core/Input/MouseEvents.h"
#include "Core/Input/KeyboardEvents.h"
#include "Core/Input/ApplicationEvents.h"

#include "Core/Renderer/Buffers.h"
#include "Core/Renderer/Shader.h"
#include "Core/Renderer/VertexArray.h"
#include "Core/Renderer/RendererAPI.h"
#include "Core/Renderer/RenderCommands.h"
#include "Core/Renderer/Renderer.h"

#include "Core/Application/Window.h"
#include "Core/Application/Application.h"
#include "Core/Application/Layer.h"

#include "Core/Imgui/ImguiLayer.h"

#ifdef QK_PLATFORM_WINDOWS
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif //QK_PLATFORM_WINDOWS


//**** Entry Point of the program ****//
// Must be the last thing in this list
#include "Core/Application/EntryPoint.h"
//**** Entry Point of the program ****//
