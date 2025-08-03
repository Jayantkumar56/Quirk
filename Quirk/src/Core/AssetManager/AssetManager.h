

#include "EditorAssetManager.h"


// NOTE: Temporary until a common define is created for editor and runtime
#define EDITOR_ASSET_MANAGER


#ifdef EDITOR_ASSET_MANAGER

namespace Quirk {

	using AssetManager = EditorAssetManager;

}

#elif defined(RUNTIME_ASSET_MANAGER)

namespace Quirk {

	// RuntimeAssetManager is not created yet
	using AssetManager = RuntimeAssetManager;

}

#endif // EDITOR_ASSET_MANAGER


