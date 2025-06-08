

#pragma once

#ifdef QK_PLATFORM_WINDOWS


#include "Core/RHI/GraphicalContext.h"


namespace Quirk::Internals {
    struct WGLExtensions;
}


namespace Quirk::OpenGL {

    class WindowsOpenGLContext : public RHI::GraphicalContext {
    public:
        static void Init();
        static inline const Internals::WGLExtensions& GetWGLExtensions() noexcept { return s_WGL; }

    public:
        virtual void CreateContext(View<Window> window)  noexcept override;
        virtual void DestroyContext(View<Window> window) noexcept override;

        virtual void SwapBuffer()          noexcept override;
        virtual void SetVSync(int toggle)  noexcept override;
        virtual void MakeContextCurrent()  noexcept override;

        inline HGLRC GetGLContext()     const noexcept { return m_GLContext;     }
        inline HDC   GetDeviceContext() const noexcept { return m_DeviceContext; }

    private:
        static Internals::WGLExtensions s_WGL;

    private:
        HDC   m_DeviceContext;
        HGLRC m_GLContext;
    };

}

#endif // QK_PLATFORM_WINDOWS
