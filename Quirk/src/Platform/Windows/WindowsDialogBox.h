

#pragma once
#ifdef QK_PLATFORM_WINDOWS

#include <filesystem>

namespace Quirk {

	struct FileFilter;
	struct FileDialogSpecification;

	class WindowsFileDialog {
	public:
		// returns true if successfully opened a file and pathOutput contains a valid path
		// returns false if an error occured or the user not selected a file
		static bool OpenFile(const FileDialogSpecification& dialogSpec, std::filesystem::path& pathOutput);

		// returns true if successful and pathOutput contains a valid path
		// returns false if an error occured or the user not selected a filename
		static bool SaveFile(const FileDialogSpecification& dialogSpec, std::filesystem::path& pathOutput);
	};

}

#endif // QK_PLATFORM_WINDOWS
