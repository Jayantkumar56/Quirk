

#pragma once


#include "WindowsOpenGLContext.h"


namespace Quirk::OpenGL {

#ifdef QK_PLATFORM_WINDOWS

    using GraphicalContext = WindowsOpenGLContext;

#endif // QK_PLATFORM_WINDOWS

}
