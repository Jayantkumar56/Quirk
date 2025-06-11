

#pragma once


#include "Core/AssetManager/EditorAssetManager.h"
#include "Core/Application/EditorProject.h"
#include "Core/Config.h"


namespace QuirkEditor {

    // NOTE:
    //
    // - ProjFilePath : path of the proj file (**.qkproj file) of the project
    // 
    // - ProjRootDirectory : directory in which proj file (**.qkproj file) of the project lives
    //                       (i.e. the root directory of the project)
    // 
    // - ProjDirectory : directory in which the folder containing proj file lives
    //
    // - Overall structure:  **/ProjDirectory/ProjRootDirectory/ProjFile
    // 
    //       example (ProjFilePath):- D:/Dev/Quirk/Example/Example.qkproj
    //       here we have:-
    //          ProjDirectory     = Quirk
    //          ProjRootDirectory = Example
    //          ProjFile          = Example.qkproj
    

	struct ProjectMetadata {
		std::string Title;
		std::filesystem::path ProjectRootDirectory;
	};

    class ProjectManager {
    public:
        void LoadRecentProjectsList(const std::filesystem::path& projListfilePath) noexcept;
        void SaveRecentProjectsList(const std::filesystem::path& projListfilePath) const;

        inline const auto& GetRecentProjectsList() const noexcept { return m_RecentProjectsList; }

        bool LoadProject       ( const std::filesystem::path& projFilePath                            );
        bool LoadProject       ( const std::string& title, const std::filesystem::path& projRootDir   );
        bool CreateInDirectory ( std::string&& title,      const std::filesystem::path& projDirectory );

        inline bool LoadProject(const ProjectMetadata& projMeta) {
            return LoadProject(projMeta.Title, projMeta.ProjectRootDirectory);
        }

        inline bool HaveRecentProjects() noexcept { return !m_RecentProjectsList.empty(); }

        // === Begin: API related to active project ===

        inline Quirk::Scope<Quirk::Project> GetActive() noexcept { return std::move(m_ActiveProject); }

        // === End:   API related to active project ===

    private:
        void CreateProjectDirectoryStructure(const std::filesystem::path& projFilePath, const Quirk::ProjectConfig& projConfig);

        void AddRecentProject(ProjectMetadata projMeta);

    private:
        Quirk::Scope<Quirk::Project> m_ActiveProject;
        std::vector<ProjectMetadata> m_RecentProjectsList;
    };

}
