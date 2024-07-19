

#pragma once

#include "Core/Core.h"
#include "Debug/Log.h"

#include "Core/Application/Window.h"
#include "Core/Application/Application.h"
#include "Core/Application/Layer.h"

#include "Core/Input/KeyCodes.h"
#include "Core/Input/Events.h"
#include "Core/Input/MouseEvents.h"
#include "Core/Input/KeyboardEvents.h"
#include "Core/Input/ApplicationEvents.h"

#ifdef QK_PLATFORM_WINDOWS
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif //QK_PLATFORM_WINDOWS


//**** Entry Point of the program ****//
// Must be the last thing in this list
#include "Core/Application/EntryPoint.h"
//**** Entry Point of the program ****//
