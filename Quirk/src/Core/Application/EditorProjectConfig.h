
#pragma once


#include <string>
#include <filesystem>

namespace Quirk {

    struct EditorProjectConfig {
        std::string ProjectName;

        // paths are relative to the project directory
        // must be converted into relative to the working directory before use

        std::filesystem::path StartScene;
        std::filesystem::path AssetDirectory;
        std::filesystem::path SceneDirectory;
        std::filesystem::path ScriptModulePath;
        std::filesystem::path AssetRegistryPath;

        static inline EditorProjectConfig GetDefaultConfig(std::string title = std::string("Untitled")) {
            return EditorProjectConfig{
                .ProjectName       { std::move(title)     },
                .StartScene        { ""                   },
                .AssetDirectory    { "Assets"             },
                .SceneDirectory    { "Assets/Scenes"      },
                .ScriptModulePath  { "Scripts"            },
                .AssetRegistryPath { "AssetRegistry.yaml" }
            };
        }
    };

}
