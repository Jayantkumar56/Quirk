

#include "Qkpch.h"

#include "RenderSystem.h"
#include "Platform/OpenGL/Factory.h"


namespace Quirk::RHI {

    void RenderSystem::InitializeHeadLess() noexcept {
        QK_CORE_ERROR("RendererAPI::None selected — no rendering backend available.");
        QK_CORE_ASSERT(false, "Currently Quirk do not support Headless build.");
    }

    void RenderSystem::InitializeWithOpenGL() noexcept {
        m_Factory = CreateScope<OpenGL::Factory>();
        OpenGL::GraphicalContext::Init();
    }

}
