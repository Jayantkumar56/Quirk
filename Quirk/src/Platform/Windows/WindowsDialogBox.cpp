


#include "Qkpch.h"

#ifdef QK_PLATFORM_WINDOWS

#include "WindowsDialogBox.h"
#include "Core/Application/Application.h"
#include "Core/Utility/DialogBox.h"


namespace Quirk {

	bool WindowsFileDialog::OpenFile(const FileDialogSpecification& dialogSpec, std::filesystem::path& pathOutput) {
		IFileOpenDialog* pFileOpen = NULL;
		HRESULT hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL, IID_IFileOpenDialog, reinterpret_cast<void**>(&pFileOpen));
			
		if (!SUCCEEDED(hr)) {
			QK_CORE_WARN("Unable to create IFileOpenDialog instance");
			return false;
		}

		// can directly convert from FileFilter* to COMDLG_FILTERSPEC* as both are the same datatypes
		hr = pFileOpen->SetFileTypes(dialogSpec.NoOfFilters, (COMDLG_FILTERSPEC*)dialogSpec.Filters);
		hr = pFileOpen->SetFileTypeIndex(0);															// Set the default filter index

		// TO DO: set default path for the dialog
		if (dialogSpec.DefaultPath != nullptr) {

		}

		if (dialogSpec.Title != nullptr)
			pFileOpen->SetTitle(dialogSpec.Title);

		if (dialogSpec.FileNameLabel != nullptr)
			pFileOpen->SetFileNameLabel(dialogSpec.FileNameLabel);

		if (dialogSpec.DefaultFileName != nullptr)
			pFileOpen->SetFileName(dialogSpec.DefaultFileName);

		HWND window = (HWND)Application::Get().GetWindow().GetNativeHandle();
		hr = pFileOpen->Show(window);
		if (!SUCCEEDED(hr)) {
			return false;
		}

		IShellItem* pItem;
		hr = pFileOpen->GetResult(&pItem);

		if (!SUCCEEDED(hr))
			return false;
		
		PWSTR pszFilePath;
		hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

		if (!SUCCEEDED(hr))
			return false;
		
		pathOutput.assign(pszFilePath);
		CoTaskMemFree(pszFilePath);

		pItem->Release();
		pFileOpen->Release();
		return true;
	}

	bool WindowsFileDialog::SaveFile(const FileDialogSpecification& dialogSpec, std::filesystem::path& pathOutput) {
		IFileSaveDialog* pFileSave = NULL;
		HRESULT hr = CoCreateInstance(CLSID_FileSaveDialog, NULL, CLSCTX_ALL, IID_IFileSaveDialog, reinterpret_cast<void**>(&pFileSave));

		if (!SUCCEEDED(hr)) {
			QK_CORE_WARN("Unable to create IFileSaveDialog instance");
			return false;
		}
	
		// can directly convert from FileFilter* to COMDLG_FILTERSPEC* as both are the same datatypes
		hr = pFileSave->SetFileTypes(dialogSpec.NoOfFilters, (COMDLG_FILTERSPEC*)dialogSpec.Filters);		
		hr = pFileSave->SetFileTypeIndex(0);																// Set the default filter index

		// TO DO: set default path for the dialog
		if (dialogSpec.DefaultPath != nullptr) {
				
		}

		if(dialogSpec.Title != nullptr)
			pFileSave->SetTitle(dialogSpec.Title);

		if(dialogSpec.FileNameLabel != nullptr)
			pFileSave->SetFileNameLabel(dialogSpec.FileNameLabel);

		if(dialogSpec.DefaultFileName != nullptr)
			pFileSave->SetFileName(dialogSpec.DefaultFileName);

		HWND window = (HWND)Application::Get().GetWindow().GetNativeHandle();
		hr = pFileSave->Show(window);
		if (!SUCCEEDED(hr)) {
			return false;
		}

		IShellItem* pItem = nullptr;
		hr = pFileSave->GetResult(&pItem);

		if (!SUCCEEDED(hr))
			return false;

		PWSTR pszFilePath;
		hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

		if (!SUCCEEDED(hr))
			return false;

		pathOutput.assign(pszFilePath);
		CoTaskMemFree(pszFilePath);

		pItem->Release();
		pFileSave->Release();
		return true;
	}

}

#endif // QK_PLATFORM_WINDOWS
