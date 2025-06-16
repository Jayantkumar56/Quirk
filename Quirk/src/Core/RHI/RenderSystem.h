

#pragma once

#include "Factory.h"
#include "GraphicsAPI.h"


namespace Quirk::RHI {

    class RenderSystem {
    public:
        explicit RenderSystem(GraphicsAPI api, View<Window> window)  noexcept;

        inline const GraphicsAPI           GetGraphicsAPI()      const noexcept { return m_GraphicsAPI; }
        inline ConstView<Factory>          GetFactory()          const noexcept { return m_Factory;     }
        inline ConstView<GraphicalContext> GetGraphicalContext() const noexcept { return m_Context;     }

    private:
        void InitializeHeadLess()   noexcept;
        void InitializeWithOpenGL() noexcept;

    private:
        GraphicsAPI             m_GraphicsAPI;
        Scope<Factory>          m_Factory;
        Scope<GraphicalContext> m_Context;
    };

}
