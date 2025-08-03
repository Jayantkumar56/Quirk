

#pragma once

#include "Core/Reflection/Reflection.h"
#include "Core/AssetManager/EditorTexture.h"


namespace Quirk {

	REGISTER_REFLECTION(EditorTexture2DSpec, "Texture2D Specification", (CONSTRUCTOR),
        ( ImagePath,    ImagePath,    DUMMY_SETTER, PROPFLAG_DIRECT_MEMBER | PROPFLAG_SERIALIZABLE ),
        ( TextureProps, TextureProps, DUMMY_SETTER, PROPFLAG_DIRECT_MEMBER | PROPFLAG_SERIALIZABLE )
    );


    REGISTER_REFLECTION(RHI::TextureProperties, "Texture2D Properties", (CONSTRUCTOR),
        ( MinFilter,    MinFilter,    DUMMY_SETTER, PROPFLAG_DIRECT_MEMBER | PROPFLAG_SERIALIZABLE ),
        ( MagFilter,    MagFilter,    DUMMY_SETTER, PROPFLAG_DIRECT_MEMBER | PROPFLAG_SERIALIZABLE ),
        ( WrapS,        WrapS,        DUMMY_SETTER, PROPFLAG_DIRECT_MEMBER | PROPFLAG_SERIALIZABLE ),
        ( WrapT,        WrapT,        DUMMY_SETTER, PROPFLAG_DIRECT_MEMBER | PROPFLAG_SERIALIZABLE ),
        ( SwizzleMask,  SwizzleMask,  DUMMY_SETTER, PROPFLAG_DIRECT_MEMBER | PROPFLAG_SERIALIZABLE )
    );


    REGISTER_REFLECTION(RHI::TextureSwizzle, "Texture2D Properties", (CONSTRUCTOR),
        ( R,   R,   DUMMY_SETTER, PROPFLAG_DIRECT_MEMBER | PROPFLAG_SERIALIZABLE ),
        ( G,   G,   DUMMY_SETTER, PROPFLAG_DIRECT_MEMBER | PROPFLAG_SERIALIZABLE ),
        ( B,   B,   DUMMY_SETTER, PROPFLAG_DIRECT_MEMBER | PROPFLAG_SERIALIZABLE ),
        ( A,   A,   DUMMY_SETTER, PROPFLAG_DIRECT_MEMBER | PROPFLAG_SERIALIZABLE )
    );

}
