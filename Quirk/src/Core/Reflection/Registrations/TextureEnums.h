

#pragma once

#include "Core/Reflection/EnumRegistry.h"
#include "Core/RHI/Texture2D.h"


namespace Quirk {

    REGISTER_ENUM( RHI::Swizzle, 
        None, 
        Red,  
        Green,
        Blue, 
        Alpha,
        Zero, 
        One 
    );


    REGISTER_ENUM( RHI::TextureFilter,
        Linear,
        Nearest,
    );


    REGISTER_ENUM( RHI::TextureWrap,
        Repeat,
        MirroredRepeat,
        ClampToEdge,
        ClampToBorder,
    );

}
