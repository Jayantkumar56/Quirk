

#pragma once

#include "FrameBuffer.h"
#include "GraphicalContext.h"
#include "Texture2D.h"


namespace Quirk::RHI {

    class Factory {
    public:
        virtual Scope<GraphicalContext> CreateGraphicalContext() const noexcept = 0;

        virtual Scope<FrameBuffer> CreateFrameBuffer(const FrameBufferSpecification& spec) const noexcept = 0;

        virtual Ref<Texture2D> CreateTexture(BufferView dataBuffer, const TextureSpec& specification) const noexcept = 0;
    };

}
