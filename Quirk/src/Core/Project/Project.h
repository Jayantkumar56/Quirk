

#pragma once

#include "Core/Core.h"
#include "Core/Application/Application.h"
#include "Core/Project/ProjectSerializer.h"

#include <string>
#include <filesystem>


namespace Quirk {

	struct ProjectConfig {
		std::string Name = "Untitled";

		// paths are relative to the project directory
		// must be converted into relative to the working directory before use

		std::filesystem::path StartScene;

		std::filesystem::path AssetDirectory;
		std::filesystem::path ScriptModulePath;
	};

	struct ProjectMetadata {
		std::string Title;
		std::string Path;
	};

	class Project {
	public:
		static inline Ref<Project> GetActive()								      noexcept { return s_ActiveProject;      }
		static inline const std::vector<ProjectMetadata>& GetRecentProjectsList() noexcept { return s_RecentProjectsList; }

		// takes path of the yaml file which contains the recent projects list
		static inline bool LoadRecentProjectsList(const std::filesystem::path& filePath) {
			return ProjectSerializer::DeserializeRecentProjectsList(s_RecentProjectsList, filePath);
		}

		// takes path of the file where recent list should be stored
		static inline bool SaveRecentProjectsList(const std::filesystem::path& filePath) {
			return ProjectSerializer::SerializeRecentProjectsList(s_RecentProjectsList, filePath);
		}

		static inline const std::filesystem::path& GetProjectDirectory() {
			QK_CORE_ASSERT(s_ActiveProject, "s_ActiveProject is NULL");
			return s_ActiveProject->m_ProjectDirectory;
		}

		static inline std::filesystem::path GetAssetDirectory() {
			QK_CORE_ASSERT(s_ActiveProject, "s_ActiveProject is NULL");
			return GetProjectDirectory() / s_ActiveProject->m_Config.AssetDirectory;
		}

		static inline std::filesystem::path GetAssetFileSystemPath(const std::filesystem::path& path) {
			QK_CORE_ASSERT(s_ActiveProject, "s_ActiveProject is NULL");
			return GetAssetDirectory() / path;
		}

		static Ref<Project> Load(const std::filesystem::path& path) {
			Ref<Project> project = CreateRef<Project>();

			if (ProjectSerializer::Deserialize(project, path)) {
				project->m_ProjectDirectory = path.parent_path();
				s_ActiveProject = project;
				return s_ActiveProject;
			}

			return nullptr;
		}

		static bool SaveActive(const std::filesystem::path& path) {
			if (ProjectSerializer::Serialize(s_ActiveProject, path)) {
				s_ActiveProject->m_ProjectDirectory = path.parent_path();
				return true;
			}

			return false;
		}

	public:
		inline ProjectConfig& GetConfig() noexcept { return m_Config; }

	private:
		ProjectConfig		  m_Config;
		std::filesystem::path m_ProjectDirectory;

		// only a single project could be loaded at once 
		inline static Ref<Project> s_ActiveProject;
		inline static std::vector<ProjectMetadata> s_RecentProjectsList;
	};

}
