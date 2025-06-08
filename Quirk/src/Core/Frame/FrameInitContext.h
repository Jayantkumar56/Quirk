

#pragma once

#include "Window.h"
#include "Core/RHI/GraphicsAPI.h"

namespace Quirk {

    // FrameInitContext : "Bag" of all possible external inputs for any FrameFeature.
    // 
    //  – derived class of FrameImpl must set exactly those fields
    //    needed by the features it requested. Unused fields can be left at default.
    // 
    //  – each policy’s constructor will assert that its required fields are set.
    struct FrameInitContext {

        //-------------------------------------------------------------------
        // for FrameFeature::Widow
        WindowSpecification WindowSpec;
        //___________________________________________________________________

        //-------------------------------------------------------------------
        // for FrameFeature::GraphicalContext
        RHI::GraphicsAPI GraphicsAPI;
        //___________________________________________________________________

    };

}
