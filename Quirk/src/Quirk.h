

#pragma once

#include "Core/Core.h"
#include "Debug/Log.h"

// Include all GLM core / GLSL features
#include "glm/glm.hpp" // vec2, vec3, mat4, radians
// Include all GLM extensions
#include "glm/ext.hpp" // perspective, translate, rotate

#include "Core/Utility/Time.h"

#include "Core/Input/KeyCodes.h"
#include "Core/Input/Events.h"
#include "Core/Input/MouseEvents.h"
#include "Core/Input/KeyboardEvents.h"
#include "Core/Input/ApplicationEvents.h"

#include "Core/Camera/OrthographicCamera.h"
#include "Core/Camera/PerspectiveCamera.h"
#include "Core/Camera/CameraController.h"

#include "Core/Shapes/Quad.h"

#include "Core/Renderer/Buffers.h"
#include "Core/Renderer/Texture.h"
#include "Core/Renderer/Shader.h"
#include "Core/Renderer/VertexArray.h"
#include "Core/Renderer/RendererAPI.h"
#include "Core/Renderer/RenderCommands.h"
#include "Core/Renderer/Renderer.h"
#include "Core/Renderer/Renderer2D.h"

#include "Core/Application/Window.h"
#include "Core/Application/Application.h"
#include "Core/Application/Layer.h"

#include "Core/Imgui/ImguiUI.h"

#ifdef QK_PLATFORM_WINDOWS
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif //QK_PLATFORM_WINDOWS

