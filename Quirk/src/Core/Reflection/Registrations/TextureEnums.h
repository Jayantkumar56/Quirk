

#pragma once

#include "Core/Reflection/EnumRegistry.h"
#include "Core/Assets/RenderAssets/Texture.h"


namespace Quirk {

    REGISTER_ENUM( Swizzle, 
        None, 
        Red,  
        Green,
        Blue, 
        Alpha,
        Zero, 
        One 
    );


    REGISTER_ENUM( TextureFilter,
        Linear,
        Nearest,
    );


    REGISTER_ENUM( TextureWrap,
        Repeat,
        MirroredRepeat,
        ClampToEdge,
        ClampToBorder,
    );

}
