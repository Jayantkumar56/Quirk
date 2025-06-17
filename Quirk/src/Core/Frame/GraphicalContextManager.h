

#pragma once

#include "Core/RHI/RenderSystem.h"
#include "Core/Renderer/Renderer.h"
#include "FrameInitContext.h"


namespace Quirk::Internals {

    class GraphicalContextManager {
    public:
        GraphicalContextManager(const FrameInitContext& initContext, auto& frame) noexcept :
                m_RenderSystem ( initContext.GraphicsAPI, frame.GetWindow() ),
                m_Renderer     ( &m_RenderSystem                            )
        {}

        inline ConstView<RHI::GraphicalContext> GetGraphicalContext()     const noexcept { return m_RenderSystem.GetGraphicalContext();     }
        inline ConstView<RHI::RenderSystem>     GetRenderSystem()         const noexcept { return &m_RenderSystem;                          }
        inline ConstView<RHI::Factory>          GetRHIFactory()           const noexcept { return m_RenderSystem.GetFactory();              }
        inline View<Renderer>                   GetRenderer()                   noexcept { return &m_Renderer;                              }
        inline ConstView<RHI::RenderCommand>    GetRenderCommandContext() const noexcept { return m_RenderSystem.GetRenderCommandContext(); }

        inline void SwapBuffer()         const noexcept { GetGraphicalContext()->SwapBuffer();     }
        inline void SetVSync(int toggle) const noexcept { GetGraphicalContext()->SetVSync(toggle); }

    private:
        RHI::RenderSystem m_RenderSystem;
        Renderer          m_Renderer;
    };

}
