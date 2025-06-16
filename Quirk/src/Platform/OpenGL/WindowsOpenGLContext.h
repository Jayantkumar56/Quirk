

#pragma once

#ifdef QK_PLATFORM_WINDOWS


#include "Core/RHI/GraphicalContext.h"


namespace Quirk::Internals {
    struct WGLExtensions;
}


namespace Quirk::OpenGL {

    class WindowsOpenGLContext final : public RHI::GraphicalContext {
    public:
        static void Init();
        static inline const Internals::WGLExtensions& GetWGLExtensions() noexcept { return s_WGL; }

    public:
        WindowsOpenGLContext(View<Window> window)  noexcept;
        ~WindowsOpenGLContext() noexcept;

        virtual void SwapBuffer()         const noexcept override;
        virtual void SetVSync(int toggle) const noexcept override;
        virtual void MakeContextCurrent() const noexcept override;

        inline HGLRC GetGLContext()     const noexcept { return m_GLContext;     }
        inline HDC   GetDeviceContext() const noexcept { return m_DeviceContext; }

    private:
        static Internals::WGLExtensions s_WGL;

    private:
        HWND  m_WindowHandle;
        HDC   m_DeviceContext;
        HGLRC m_GLContext;
    };

}

#endif // QK_PLATFORM_WINDOWS
