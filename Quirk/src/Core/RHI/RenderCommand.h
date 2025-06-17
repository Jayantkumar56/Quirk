

#pragma once

#include "VertexArray.h"

#include "glm/glm.hpp"


namespace Quirk::RHI {

    enum class CullMode {
        None,
        Front,
        Back
    };

	class RenderCommand {
	public:
        // BEGIN Frame Setup Commands --------------------------

        virtual void SetClearColor(const glm::vec4& color)                             const noexcept = 0;
        virtual void Clear(bool color = true, bool depth = true, bool stencil = false) const noexcept = 0;
        virtual void SetViewport(uint32_t width, uint32_t height)                      const noexcept = 0;

        // END Frame Setup Commands ----------------------------

        // BEGIN Render State Control --------------------------

        virtual void EnableBlending()           const noexcept = 0;
        virtual void EnableDepthTesting()       const noexcept = 0;
        virtual void SetCullMode(CullMode mode) const noexcept = 0;

        // END Render State Control ----------------------------

        // BEGIN Draw Commands ---------------------------------

        virtual void DrawIndexed(Ref<VertexArray> vertexArray, uint32_t indexCount)             const noexcept = 0;
        virtual void DrawIndexedInstanced(Ref<VertexArray> vertexArray, uint32_t instanceCount) const noexcept = 0;
        virtual void DrawVertices(uint32_t vertexCount)                                         const noexcept = 0;

        // END Draw Commands -----------------------------------
	};

}
