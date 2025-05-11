
#pragma once


#include <string>
#include <filesystem>

namespace Quirk {

    struct ProjectConfig {
        std::string Name;

        // paths are relative to the project directory
        // must be converted into relative to the working directory before use

        std::filesystem::path StartScene;
        std::filesystem::path AssetDirectory;
        std::filesystem::path SceneDirectory;
        std::filesystem::path ScriptModulePath;
        std::filesystem::path AssetRegistryPath;

        static inline ProjectConfig GetDefaultConfig(std::string&& title = std::string("Untitled")) {
            return ProjectConfig{
                .Name              { std::move(title)     },
                .StartScene        { ""                   },
                .AssetDirectory    { "Assets"             },
                .SceneDirectory    { "Assets/Scenes"      },
                .ScriptModulePath  { "Scripts"            },
                .AssetRegistryPath { "AssetRegistry.yaml" }
            };
        }

        static inline ProjectConfig GetDefaultConfig(const std::string& title) {
            return GetDefaultConfig(std::string(title));
        }
    };

}
