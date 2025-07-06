

#pragma once

#ifdef QK_PLATFORM_WINDOWS


#include "glad/glad.h"
#include "wglext.h"


namespace Quirk::Internals {

    struct WGLExtensions {
        HINSTANCE                           OpenGL32DLL               = nullptr;
        PFNWGLCHOOSEPIXELFORMATARBPROC		ChoosePixelFormatARB      = nullptr;
        PFNWGLSWAPINTERVALEXTPROC           SwapIntervalEXT           = nullptr;
        PFNWGLGETPIXELFORMATATTRIBIVARBPROC GetPixelFormatAttribivARB = nullptr;
        PFNWGLGETEXTENSIONSSTRINGEXTPROC    GetExtensionsStringEXT    = nullptr;
        PFNWGLGETEXTENSIONSSTRINGARBPROC    GetExtensionsStringARB    = nullptr;
        PFNWGLCREATECONTEXTATTRIBSARBPROC   CreateContextAttribsARB   = nullptr;
    };

}

#endif // QK_PLATFORM_WINDOWS
