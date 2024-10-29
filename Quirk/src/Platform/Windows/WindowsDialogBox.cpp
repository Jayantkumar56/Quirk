


#include "Qkpch.h"

#ifdef QK_PLATFORM_WINDOWS

#include "WindowsDialogBox.h"
#include "Core/Application/Application.h"
#include "Core/Utility/DialogBox.h"

namespace Quirk {

	std::wstring WindowsFileDialog::OpenFile(const FileDialogSpecification& dialogSpec) {
		std::wstring filePath;
		IFileOpenDialog* pFileOpen = NULL;
		HRESULT hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL, IID_IFileOpenDialog, reinterpret_cast<void**>(&pFileOpen));
			
		if (SUCCEEDED(hr)) {
			// can directly cas from FileFilter* to COMDLG_FILTERSPEC* as both are the same datatypes
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

			hr = pFileOpen->Show(NULL);

			if (SUCCEEDED(hr)) {
				IShellItem* pItem;
				hr = pFileOpen->GetResult(&pItem);

				if (SUCCEEDED(hr)) {
					PWSTR pszFilePath;
					hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

					if (SUCCEEDED(hr)) {
						filePath = std::wstring(pszFilePath);
						CoTaskMemFree(pszFilePath);
					}

					pItem->Release();
				}
			}

			pFileOpen->Release();
		}

		return filePath;
	}

	std::wstring WindowsFileDialog::SaveFile(const FileDialogSpecification& dialogSpec) {
		std::wstring filePath;
		IFileSaveDialog* pFileSave = NULL;
		HRESULT hr = CoCreateInstance(CLSID_FileSaveDialog, NULL, CLSCTX_ALL, IID_IFileSaveDialog, reinterpret_cast<void**>(&pFileSave));

		if (SUCCEEDED(hr)) {
			// can directly cas from FileFilter* to COMDLG_FILTERSPEC* as both are the same datatypes
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

			hr = pFileSave->Show(NULL);

			if (SUCCEEDED(hr)) {
				IShellItem* pItem = nullptr;
				hr = pFileSave->GetResult(&pItem);

				if (SUCCEEDED(hr)) {
					PWSTR pszFilePath;
					hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

					if (SUCCEEDED(hr)) {
						filePath = std::wstring(pszFilePath);
						CoTaskMemFree(pszFilePath);
					}

					pItem->Release();
				}
			}

			pFileSave->Release();
		}

		return filePath;
	}

}

#endif // QK_PLATFORM_WINDOWS
