

#pragma once

#include "Platform/Windows/WindowsDialogBox.h"

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
		FileFilter*		Filters; 
		unsigned int	NoOfFilters;
	};

	class FileDialog {
	public:

		static inline std::wstring OpenFile(const FileDialogSpecification& dialogSpec) {
#ifdef QK_PLATFORM_WINDOWS
			return WindowsFileDialog::OpenFile(dialogSpec);
#endif // QK_PLATFORM_WINDOWS
		}

		static inline std::wstring SaveFile(const FileDialogSpecification& dialogSpec) {
#ifdef QK_PLATFORM_WINDOWS
			return WindowsFileDialog::SaveFile(dialogSpec);
#endif // QK_PLATFORM_WINDOWS
		}

	};

}

