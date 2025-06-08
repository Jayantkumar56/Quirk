

#pragma once

#include "Core/RHI/Factory.h"
#include "GraphicalContext.h"

namespace Quirk::OpenGL {

    class Factory : public RHI::Factory {
    public:
        virtual Scope<RHI::GraphicalContext> CreateGraphicalContext() const noexcept override {
            return CreateScope<OpenGL::GraphicalContext>();
        }
    };

}
