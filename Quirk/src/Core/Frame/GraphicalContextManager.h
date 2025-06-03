

#pragma once

#include "GraphicalContext.h"
#include "Core/Utility/View.h"

namespace Quirk {

    class GraphicalContextManager {
    public:
        GraphicalContextManager(Window& window) : 
                m_Context(GraphicalContext::Create(window)) 
        {}

        inline GraphicalContext* GetGraphicalContext() const noexcept {
            return m_Context.get();
        }

        inline void SwapBuffer() const   { m_Context->SwapBuffer();     }
        inline void SetVSync(int toggle) { m_Context->SetVSync(toggle); }

    private:
        Scope<GraphicalContext> m_Context;
    };

}
