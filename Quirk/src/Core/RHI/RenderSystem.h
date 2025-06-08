

#pragma once

#include "Factory.h"
#include "GraphicsAPI.h"

namespace Quirk::RHI {

    class RenderSystem {
    public:
        explicit RenderSystem(GraphicsAPI api)  noexcept :
                m_RendererAPI(api)
        {
            switch (api) {
                case GraphicsAPI::None:      InitializeHeadLess();       break;
                case GraphicsAPI::OpenGL:    InitializeWithOpenGL();     break;

                default: QK_CORE_ASSERT(false, "Unknown RendererAPI");
            }
        }

        inline ConstView<Factory> GetFactory()     const noexcept { return m_Factory;     }
        inline const GraphicsAPI  GetRendererAPI() const noexcept { return m_RendererAPI; }

    private:
        void InitializeHeadLess()   noexcept;
        void InitializeWithOpenGL() noexcept;

    private:
        GraphicsAPI    m_RendererAPI;
        Scope<Factory> m_Factory;
    };

}
