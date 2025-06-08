

#pragma once

#include "GraphicalContext.h"


namespace Quirk::RHI {

    class Factory {
    public:
        virtual Scope<GraphicalContext> CreateGraphicalContext() const noexcept = 0;
    };

}
