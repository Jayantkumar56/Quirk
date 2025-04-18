

#pragma once

#include "Core/Core.h"
#include "Core/Project/ProjectSerializer.h"
#include "Core/AssetManager/AssetManagerBase.h"

#include <string>
#include <filesystem>

namespace Quirk {

	struct ProjectConfig {
		std::string Name = "Untitled";

		// paths are relative to the project directory
		// must be converted into relative to the working directory before use

		std::filesystem::path StartScene;
		std::filesystem::path AssetDirectory;
		std::filesystem::path SceneDirectory;
		std::filesystem::path ScriptModulePath;
		std::filesystem::path AssetRegistryPath;

        static inline ProjectConfig GetDefaultConfig(std::string&& title) {
            return ProjectConfig {
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

	class Project {
	private:
        // Private constructor to prevent direct instantiation. Use Create() or Load() instead.
        // 
        // takes the ownership of the passed AssetManagerBase* in a Ref member varaible
        // should not be called directly (if called should consider ownership of the parameters)
        inline Project(std::filesystem::path projRootDir, ProjectConfig config, AssetManagerBase* assetManager) noexcept :
                m_Config              ( std::move(config)      ),
                m_ProjectRootDirectory( std::move(projRootDir) ),
                m_AssetManager        ( assetManager           )
        {
        }

    public:
        template<AssetManagerType T, typename PathType>
        requires std::same_as<std::remove_cvref_t<PathType>, std::filesystem::path>
        static inline Ref<Project> Create(PathType&& projRootDir, ProjectConfig&& config) {
            return Ref<Project>(new Project(std::forward<PathType>(projRootDir), std::move(config), new T));
        }

        template<AssetManagerType T>
		static inline Ref<Project> Load(const std::filesystem::path& projFilePath) {
            ProjectConfig config;
            if (!ProjectSerializer::DeserializeConfig(config, projFilePath)) {
                QK_WARN("Unable to Deserialize proj file!");
                return nullptr;
            }

            return Ref<Project>(new Project(projFilePath.parent_path(), std::move(config), new T));
		}

        static inline std::string_view GetProjFileExtenstion() noexcept { return ".qkproj"; }

    public:
        inline bool Save(const std::filesystem::path& projDirectory) {
            std::filesystem::path projRootDir = projDirectory / m_Config.Name;
            std::string projFile;

            // setting projfile name
            {
                std::string_view extension = Project::GetProjFileExtenstion();
                projFile.reserve(m_Config.Name.size() + extension.size());
                projFile += m_Config.Name;
                projFile += extension;
            }

            if (ProjectSerializer::Serialize(Ref<Project>(this), projRootDir / projFile)) {
                m_ProjectRootDirectory = projRootDir;
                return true;
            }

            return false;
        }

        inline auto  GetTitle()     const noexcept { return m_Config.Name;          }
        inline auto& GetConfig()          noexcept { return m_Config;               }
        inline auto  GetDirectory() const noexcept { return m_ProjectRootDirectory; }

        inline const auto& GetTitle()        noexcept { return m_Config.Name;          }
        inline const auto& GetConfig() const noexcept { return m_Config;               }
        inline const auto& GetDirectory()    noexcept { return m_ProjectRootDirectory; }

        inline auto  GetAssetManager() const noexcept { return m_AssetManager; }
        inline auto& GetAssetManager()       noexcept { return m_AssetManager; }

		inline auto GetAssetDirectory() const noexcept { 
            return m_ProjectRootDirectory / m_Config.AssetDirectory; 
        }

		inline auto GetAssetFileSystemPath(const std::filesystem::path& path) const noexcept {
			return GetAssetDirectory() / path;
		}

        inline void SetRootDirectory(const std::filesystem::path& projRoot) noexcept { 
            m_ProjectRootDirectory = projRoot; 
        }

	private:
		ProjectConfig		  m_Config;
		std::filesystem::path m_ProjectRootDirectory;
        Ref<AssetManagerBase> m_AssetManager;
	};

}
