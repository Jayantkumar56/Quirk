

#pragma once

#include "Core/RHI/RenderCommand.h"


namespace Quirk::OpenGL {

	class RenderCommand final : public RHI::RenderCommand {
    public:
        // BEGIN: Frame Setup Commands --------------------------

        virtual void SetClearColor(const glm::vec4& color)                             const noexcept override;
        virtual void Clear(bool color = true, bool depth = true, bool stencil = false) const noexcept override;
        virtual void SetViewport(uint32_t width, uint32_t height)                      const noexcept override;

        // END: Frame Setup Commands ----------------------------

        // BEGIN: Render State Control --------------------------

        virtual void EnableBlending()                const noexcept override;
        virtual void EnableDepthTesting()            const noexcept override;
        virtual void SetCullMode(RHI::CullMode mode) const noexcept override;

        // END: Render State Control ----------------------------

        // BEGIN: Draw Commands ---------------------------------

        virtual void DrawIndexed(Ref<RHI::VertexArray> vertexArray, uint32_t indexCount)             const noexcept override;
        virtual void DrawIndexedInstanced(Ref<RHI::VertexArray> vertexArray, uint32_t instanceCount) const noexcept override;
        virtual void DrawVertices(uint32_t vertexCount)                                              const noexcept override;

        // END: Draw Commands -----------------------------------
	};

}
