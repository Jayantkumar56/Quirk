

#pragma once

#include "Platform/Windows/WindowsDialogBox.h"
#include "Core/Application/Window.h"

namespace Quirk {

	// Does not take responsibility of the pointers 
	struct FileFilter {
		const wchar_t* Name;
		const wchar_t* Type;
	};

	// Does not take responsibility of the pointers 
	struct FileDialogSpecification {
		const wchar_t*	Title		    = nullptr;
		const wchar_t*  DefaultPath     = nullptr;
		const wchar_t*  FileNameLabel   = nullptr;
		const wchar_t*	DefaultFileName = nullptr;
		Window*         ParentWindow    = nullptr;
		FileFilter*		Filters			= nullptr; 
		unsigned int	NoOfFilters		= 0;
	};

	class FileDialog {
	public:

		static inline bool OpenFile(const FileDialogSpecification& dialogSpec, std::filesystem::path& pathOutput) {
#ifdef QK_PLATFORM_WINDOWS
			return WindowsFileDialog::OpenFile(dialogSpec, pathOutput);
#endif // QK_PLATFORM_WINDOWS
		}

		static inline bool SaveFile(const FileDialogSpecification& dialogSpec, std::filesystem::path& pathOutput) {
#ifdef QK_PLATFORM_WINDOWS
			return WindowsFileDialog::SaveFile(dialogSpec, pathOutput);
#endif // QK_PLATFORM_WINDOWS
		}

		static inline bool OpenFolder(const FileDialogSpecification& dialogSpec, std::filesystem::path& pathOutput) {
#ifdef QK_PLATFORM_WINDOWS
			return WindowsFileDialog::OpenFolder(dialogSpec, pathOutput);
#endif // QK_PLATFORM_WINDOWS
		}

	};

}

