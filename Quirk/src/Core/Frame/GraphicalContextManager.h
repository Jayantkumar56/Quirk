

#pragma once

#include "GraphicalContext.h"
#include "Core/Utility/View.h"
#include "FrameInitContext.h"

namespace Quirk::Internals {

    class GraphicalContextManager {
    public:
        GraphicalContextManager(const FrameInitContext& initContext, auto& frame) :
                m_Context(GraphicalContext::Create(frame.GetWindow()))
        {
            m_Context->MakeContextCurrent();
        }

        inline View<GraphicalContext> GetGraphicalContext() const noexcept {
            return m_Context.get();
        }

        inline void SwapBuffer() const   { m_Context->SwapBuffer();     }
        inline void SetVSync(int toggle) { m_Context->SetVSync(toggle); }

    private:
        Scope<GraphicalContext> m_Context;
    };

}
