

#pragma once

#include "Core/RHI/Factory.h"
#include "FrameBuffer.h"
#include "GraphicalContext.h"
#include "Texture2D.h"


namespace Quirk::OpenGL {

    class Factory : public RHI::Factory {
    public:
        virtual Scope<RHI::GraphicalContext> CreateGraphicalContext() const noexcept override {
            return CreateScope<OpenGL::GraphicalContext>();
        }

        virtual Scope<RHI::FrameBuffer> CreateFrameBuffer(const RHI::FrameBufferSpecification& spec) const noexcept override {
            return CreateScope<OpenGL::FrameBuffer>(spec);
        }

        virtual Ref<RHI::Texture2D> CreateTexture(BufferView dataBuffer, const RHI::TextureSpec& specification) const noexcept override {
            return CreateRef<OpenGL::Texture2D>(dataBuffer, specification);
        }
    };

}
