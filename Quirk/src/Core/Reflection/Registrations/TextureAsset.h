

#include "Core/Reflection/Reflection.h"
#include "Core/Assets/TextureAsset.h"


namespace Quirk {

	REGISTER_REFLECTION(TextureAsset, "TextureAsset", (CONSTRUCTOR),
        ( SourcePath,  GetSourcePath,  DUMMY_SETTER, PROPFLAG_SERIALIZABLE ),
        ( MinFilter,   GetMinFilter,   DUMMY_SETTER, PROPFLAG_SERIALIZABLE ),
        ( MagFilter,   GetMagFilter,   DUMMY_SETTER, PROPFLAG_SERIALIZABLE ),
        ( WrapS,       GetWrapS,       DUMMY_SETTER, PROPFLAG_SERIALIZABLE ),
        ( WrapT,       GetWrapT,       DUMMY_SETTER, PROPFLAG_SERIALIZABLE ),
        ( SwizzleMask, GetSwizzleMask, DUMMY_SETTER, PROPFLAG_SERIALIZABLE )
    );


    REGISTER_REFLECTION(RHI::TextureSwizzle, "Texture2D Properties", (CONSTRUCTOR),
        ( R,   R,   DUMMY_SETTER, PROPFLAG_DIRECT_MEMBER | PROPFLAG_SERIALIZABLE ),
        ( G,   G,   DUMMY_SETTER, PROPFLAG_DIRECT_MEMBER | PROPFLAG_SERIALIZABLE ),
        ( B,   B,   DUMMY_SETTER, PROPFLAG_DIRECT_MEMBER | PROPFLAG_SERIALIZABLE ),
        ( A,   A,   DUMMY_SETTER, PROPFLAG_DIRECT_MEMBER | PROPFLAG_SERIALIZABLE )
    );

}
