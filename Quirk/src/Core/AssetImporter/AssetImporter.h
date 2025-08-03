

#pragma once

#include "Core/Utility/SmartPointer.h"
#include "Core/Utility/View.h"


namespace Quirk {

    namespace RHI {
        class Factory;
    }

    // NOTE:
    //
    // - every AssetImporter specialization must have the following function signature and must define:
    // 

    template<typename T>
    class AssetImporter {
    public:
        static Ref<T> CreateFromSource(const std::filesystem::path& filePath, ConstView<RHI::Factory> factory)  {}
        static Ref<T> Import(const std::filesystem::path& assetPath, ConstView<RHI::Factory> factory)           {}
        static bool   Save(ConstView<T> asset, const std::filesystem::path& assetPath)                          {}

        static_assert(AlwaysFalse_V<T>, "No importer exist for given Asset type!");
    };

}
 