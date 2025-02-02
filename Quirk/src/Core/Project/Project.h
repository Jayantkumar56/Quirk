

#pragma once

#include "Core/Core.h"

#include <string>
#include <filesystem>


namespace Quirk {

	struct ProjectConfig {
		std::string Name = "Untitled";

		std::filesystem::path StartScene;

		std::filesystem::path AssetDirectory;
		std::filesystem::path ScriptModulePath;
	};


	class Project {
	public:
		static const std::filesystem::path& GetProjectDirectory() {
			QK_CORE_ASSERT(s_ActiveProject, "s_ActiveProject is NULL");
			return s_ActiveProject->m_ProjectDirectory;
		}

		static std::filesystem::path GetAssetDirectory() {
			QK_CORE_ASSERT(s_ActiveProject, "s_ActiveProject is NULL");
			return GetProjectDirectory() / s_ActiveProject->m_Config.AssetDirectory;
		}

		static std::filesystem::path GetAssetFileSystemPath(const std::filesystem::path& path) {
			QK_CORE_ASSERT(s_ActiveProject, "s_ActiveProject is NULL");
			return GetAssetDirectory() / path;
		}

		ProjectConfig& GetConfig()      { return m_Config;        }
		static Ref<Project> GetActive() { return s_ActiveProject; }

		static Ref<Project> New();
		static Ref<Project> Load(const std::filesystem::path& path);
		static bool SaveActive(const std::filesystem::path& path);

	private:
		ProjectConfig m_Config;
		std::filesystem::path m_ProjectDirectory;

		inline static Ref<Project> s_ActiveProject;
	};

}
