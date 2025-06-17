

#include "Qkpch.h"

#include "RenderSystem.h"
#include "Platform/OpenGL/OpenGLFactory.h"


namespace Quirk::RHI {

    RenderSystem::RenderSystem(GraphicsAPI api, View<Window> window) noexcept :
            m_GraphicsAPI(api)
    {
        switch (api) {
            case GraphicsAPI::None:      InitializeHeadLess();       break;
            case GraphicsAPI::OpenGL:    InitializeWithOpenGL();     break;

            default: QK_CORE_ASSERT(false, "Unknown RendererAPI");
        }

        m_Context  = m_Factory->CreateGraphicalContext(window);
        m_Commands = m_Factory->CreateRenderCommandContext();
    }

    void RenderSystem::InitializeHeadLess() noexcept {
        QK_CORE_ERROR("RendererAPI::None selected — no rendering backend available.");
        QK_CORE_ASSERT(false, "Currently Quirk do not support Headless build.");
    }

    void RenderSystem::InitializeWithOpenGL() noexcept {
        m_Factory = CreateScope<OpenGL::Factory>();
        OpenGL::GraphicalContext::Init();
    }

}
