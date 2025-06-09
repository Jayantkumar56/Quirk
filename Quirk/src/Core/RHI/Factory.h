

#pragma once

#include "GraphicalContext.h"
#include "FrameBuffer.h"


namespace Quirk::RHI {

    class Factory {
    public:
        virtual Scope<GraphicalContext> CreateGraphicalContext() const noexcept = 0;

        virtual Scope<FrameBuffer> CreateFrameBuffer(const FrameBufferSpecification& spec) const noexcept = 0;
    };

}
