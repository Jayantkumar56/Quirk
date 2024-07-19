

#include "Qkpch.h"

#define IMGUI_IMPL_OPENGL_LOADER_GLAD

#pragma warning(push)
// disabled warning for sscanf, strcpy usage in "backends/imgui_impl_opengl3.cpp"
#pragma warning(disable: 4996)

#include "backends/imgui_impl_win32.cpp"
#include "backends/imgui_impl_opengl3.cpp"

#pragma warning(pop)

