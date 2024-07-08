

#pragma once

#include "Debug/Log.h"
#include "Core/core.h"
#include "Core/Window.h"
#include "Core/Application.h"

#include "Core/Input/KeyCodes.h"

#ifdef QK_PLATFORM_WINDOWS
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif //QK_PLATFORM_WINDOWS


//**** Entry Point of the program ****//
// Must be the last thing in this list
#include "Core/EntryPoint.h"
//**** Entry Point of the program ****//
