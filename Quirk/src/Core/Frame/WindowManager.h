

#pragma once

#include "Window.h"
#include "FrameInitContext.h"


namespace Quirk::Internals {

    class WindowManager {
    public:
        WindowManager(const FrameInitContext& initContext, auto& frame) noexcept : 
                m_Window(initContext.WindowSpec) 
        {}

        inline View<Window> GetWindow() noexcept { return &m_Window; }

    private:
        Window m_Window;
    };

}
