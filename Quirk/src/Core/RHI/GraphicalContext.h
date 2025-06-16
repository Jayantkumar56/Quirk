

#pragma once

#include "Core/Frame/Window.h"
#include "Core/Utility/View.h"


namespace Quirk::RHI {

    class GraphicalContext {
    public:
        virtual void SwapBuffer()         const noexcept = 0;
        virtual void SetVSync(int toggle) const noexcept = 0;
        virtual void MakeContextCurrent() const noexcept = 0;
    };

}
