

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
        // takes the ownership of the passed AssetManagerBase* in a Ref member varaible
        // should not be called directly (if called should consider ownership of the parameters)
        inline Project(std::filesystem::path projRootDir, ProjectConfig&& config, AssetManagerBase* assetManager) noexcept :
                m_Config              ( std::move(config)      ),
                m_ProjectRootDirectory( std::move(projRootDir) ),
                m_AssetManager        ( assetManager           )
        {
        }

    public:
        template<AssetManagerType T, typename PathType>
        requires std::same_as<std::remove_cvref_t<PathType>, std::filesystem::path>
        static inline Ref<Project> Create(PathType&& projRootDir, ProjectConfig&& config) {
            s_ActiveProject = Ref<Project>(new Project(std::forward<PathType>(projRootDir), std::move(config), new T));
            return s_ActiveProject;
        }

        template<AssetManagerType T>
		static inline Ref<Project> Load(const std::filesystem::path& projFilePath) {
            ProjectConfig config;
            if (!ProjectSerializer::DeserializeConfig(config, projFilePath)) {
                QK_WARN("Unable to Deserialize proj file!");
                return nullptr;
            }

            s_ActiveProject = Ref<Project>(new Project(projFilePath.parent_path(), std::move(config), new T));
			return s_ActiveProject;
		}

		static bool SaveActive(const std::filesystem::path& path) {
			if (ProjectSerializer::Serialize(s_ActiveProject, path)) {
				s_ActiveProject->m_ProjectRootDirectory = path.parent_path();
				return true;
			}

			return false;
		}

		static inline auto  GetActive()       noexcept { return s_ActiveProject;                 }
        static inline auto& GetConfig()       noexcept { return s_ActiveProject->m_Config;       }
        static inline auto  GetAssetManager() noexcept { return s_ActiveProject->m_AssetManager; }

        static inline std::string GetTitle() noexcept {
            QK_CORE_ASSERT(s_ActiveProject, "No Active Project!");
            return s_ActiveProject->m_Config.Name;
        }

        static inline const auto& GetDirectory() noexcept {
			QK_CORE_ASSERT(s_ActiveProject, "No Active Project!");
            return s_ActiveProject->m_ProjectRootDirectory;
        }

		static inline auto GetAssetDirectory() noexcept {
			QK_CORE_ASSERT(s_ActiveProject, "No Active Project!");
			return s_ActiveProject->m_ProjectRootDirectory / s_ActiveProject->m_Config.AssetDirectory;
		}

		static inline auto GetAssetFileSystemPath(const std::filesystem::path& path) noexcept {
			QK_CORE_ASSERT(s_ActiveProject, "No Active Project!");
			return GetAssetDirectory() / path;
		}

        static inline std::string_view GetProjFileExtenstion() noexcept { return ".qkproj"; }

	private:
		ProjectConfig		  m_Config;
		std::filesystem::path m_ProjectRootDirectory;
        Ref<AssetManagerBase> m_AssetManager;

		// only a single project could be loaded at once
		static Ref<Project> s_ActiveProject;
	};

}
