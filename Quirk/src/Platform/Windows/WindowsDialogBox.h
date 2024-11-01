

#pragma once
#ifdef QK_PLATFORM_WINDOWS

#include <string>
#include <shobjidl.h>

namespace Quirk {

	struct FileFilter;
	struct FileDialogSpecification;

	class WindowsFileDialog {
	public:
		static std::wstring OpenFile(const FileDialogSpecification& dialogSpec);
		static std::wstring SaveFile(const FileDialogSpecification& dialogSpec);
	};

}

#endif // QK_PLATFORM_WINDOWS
