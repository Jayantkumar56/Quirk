

#pragma once

#include "Core/Frame/Window.h"
#include "Core/Utility/View.h"


namespace Quirk::RHI {

    class GraphicalContext {
    public:
        virtual void CreateContext(View<Window> window)  noexcept = 0;
        virtual void DestroyContext(View<Window> window) noexcept = 0;

        virtual void SwapBuffer()          noexcept = 0;
        virtual void SetVSync(int toggle)  noexcept = 0;
        virtual void MakeContextCurrent()  noexcept = 0;
    };

}
