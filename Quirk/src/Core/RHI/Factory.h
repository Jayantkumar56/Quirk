

#pragma once

#include "FrameBuffer.h"
#include "GraphicalContext.h"
#include "Texture2D.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "RenderCommand.h"


namespace Quirk::RHI {

    class Factory {
    public:
        virtual Scope<GraphicalContext> CreateGraphicalContext(View<Window> window) const noexcept = 0;

        virtual Scope<FrameBuffer> CreateFrameBuffer(const FrameBufferSpecification& spec) const noexcept = 0;

        virtual Ref<Texture2D> CreateTexture(BufferView dataBuffer, const TextureSpec& specification) const noexcept = 0;

        virtual Ref<VertexBuffer> CreateVertexBuffer()                  const noexcept = 0;
        virtual Ref<VertexBuffer> CreateVertexBuffer(BufferView buffer) const noexcept = 0;
        virtual Ref<VertexBuffer> CreateVertexBuffer(uint32_t size)     const noexcept = 0;

        virtual Ref<IndexBuffer> CreateIndexBuffer(uint32_t* indices, uint32_t count) const noexcept = 0;

        virtual Ref<VertexArray> CreateVertexArray() const noexcept = 0;

        virtual Scope<RenderCommand> CreateRenderCommandContext() const noexcept = 0;
    };

}
