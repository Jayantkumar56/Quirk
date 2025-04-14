

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
	};

	// path of projMetaData should point to the directory where the proj file (**.qkproj) lives
	// eg. for Title = Untitled
	//         Path  = C:\Document\Untitled
	// thus Untitled.qkproj file will reside like C:\Document\Untitled\Untitled.qkproj
	struct ProjectMetadata {
		std::string Title;
		std::filesystem::path Path;
	};

	class Project {
	public:
        template<AssetManagerType T>
        static Ref<Project> Create(const std::filesystem::path& path, ProjectConfig&& config) {
            s_ActiveProject = CreateRef<Project>();

            s_ActiveProject->m_ProjectDirectory = path.parent_path();
            s_ActiveProject->m_Config           = std::move(config);
            s_ActiveProject->m_AssetManager     = CreateRef<T>();

            return s_ActiveProject;
        }

        template<AssetManagerType T>
		static Ref<Project> Load(const std::filesystem::path& path) {
            s_ActiveProject = CreateRef<Project>();
            s_ActiveProject->m_ProjectDirectory = path.parent_path();

			if (!ProjectSerializer::Deserialize(s_ActiveProject, path)) {
                s_ActiveProject = nullptr;
			}

            s_ActiveProject->m_AssetManager = CreateRef<T>();

			return s_ActiveProject;
		}

		static bool SaveActive(const std::filesystem::path& path) {
			if (ProjectSerializer::Serialize(s_ActiveProject, path)) {
				s_ActiveProject->m_ProjectDirectory = path.parent_path();
				return true;
			}

			return false;
		}

		static inline auto  GetActive()       noexcept { return s_ActiveProject;                 }
        static inline auto& GetConfig()       noexcept { return s_ActiveProject->m_Config;       }
        static inline auto  GetAssetManager() noexcept { return s_ActiveProject->m_AssetManager; }

        static inline const auto& GetDirectory() noexcept {
			QK_CORE_ASSERT(s_ActiveProject, "No Active Project!");
            return s_ActiveProject->m_ProjectDirectory;
        }

		static inline auto GetAssetDirectory() {
			QK_CORE_ASSERT(s_ActiveProject, "No Active Project!");
			return s_ActiveProject->m_ProjectDirectory / s_ActiveProject->m_Config.AssetDirectory;
		}

		static inline std::filesystem::path GetAssetFileSystemPath(const std::filesystem::path& path) {
			QK_CORE_ASSERT(s_ActiveProject, "No Active Project!");
			return GetAssetDirectory() / path;
		}

	private:
		ProjectConfig		  m_Config;
		std::filesystem::path m_ProjectDirectory;
        Ref<AssetManagerBase> m_AssetManager;

		// only a single project could be loaded at once
		static Ref<Project> s_ActiveProject;
	};

}
