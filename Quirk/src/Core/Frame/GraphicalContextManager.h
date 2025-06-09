

#pragma once

#include "Core/RHI/RenderSystem.h"
#include "FrameInitContext.h"


namespace Quirk::Internals {

    class GraphicalContextManager {
    public:
        GraphicalContextManager(const FrameInitContext& initContext, auto& frame) noexcept :
                m_Window       ( frame.GetWindow()                                     ),
                m_RenderSystem ( initContext.GraphicsAPI                               ),
                m_Context      ( m_RenderSystem.GetFactory()->CreateGraphicalContext() )
        {
            m_Context->CreateContext(m_Window);
        }

        ~GraphicalContextManager() noexcept {
            m_Context->DestroyContext(m_Window);
        }

        inline View<RHI::GraphicalContext> GetGraphicalContext() const noexcept {
            return m_Context.get();
        }

        inline ConstView<RHI::RenderSystem> GetRenderSystem() const noexcept {
            return &m_RenderSystem;
        }

        inline void SwapBuffer()   const noexcept { m_Context->SwapBuffer();     }
        inline void SetVSync(int toggle) noexcept { m_Context->SetVSync(toggle); }

    private:
        View<Window>      m_Window;
        RHI::RenderSystem m_RenderSystem;

        Scope<RHI::GraphicalContext> m_Context;
    };

}
