

#pragma once

#include "Window.h"


namespace Quirk {

    class WindowManager {
    public:
        WindowManager(const WindowSpecification& spec) : 
                m_Window(spec) 
        {}

        inline Window& GetWindow() noexcept { return m_Window; }

    private:
        Window m_Window;
    };

}
