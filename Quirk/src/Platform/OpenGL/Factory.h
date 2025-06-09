

#pragma once

#include "Core/RHI/Factory.h"
#include "GraphicalContext.h"
#include "FrameBuffer.h"


namespace Quirk::OpenGL {

    class Factory : public RHI::Factory {
    public:
        virtual Scope<RHI::GraphicalContext> CreateGraphicalContext() const noexcept override {
            return CreateScope<OpenGL::GraphicalContext>();
        }

        virtual Scope<RHI::FrameBuffer> CreateFrameBuffer(const RHI::FrameBufferSpecification& spec) const noexcept override {
            return CreateScope<OpenGL::FrameBuffer>(spec);
        }
    };

}
