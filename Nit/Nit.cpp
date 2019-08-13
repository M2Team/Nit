/*
 * PROJECT:   Nit - A Windows Image Tweaker based on DismCore
 * FILE:      Nit.cpp
 * PURPOSE:   Implementation for Nit
 *
 * LICENSE:   The MIT License
 *
 * DEVELOPER: Mouri_Naruto (Mouri_Naruto AT Outlook.com)
 */

#include <Windows.h>

#include <cstdint>

// mouri::os_type::windows
// mouri::platform_error
// mouri::platform_exception

class CWin32Error
{
private:
    DWORD m_Value = ERROR_SUCCESS;

public:
    void FromWin32Error(
        _In_ DWORD Value)
    {
        this->m_Value = Value;
    }

    void FromLastError()
    {
        this->m_Value = ::GetLastError();
    }

    void FromCppBool(
        _In_ bool Value)
    {
        this->m_Value = Value ? ERROR_SUCCESS : ERROR_INVALID_FUNCTION;
    }

    void FromWin32Bool(
        _In_ BOOL Value)
    {
        // https://docs.microsoft.com/en-us/windows/win32/winprog/windows-data-types
        this->FromCppBool(Value == TRUE);
    }

    void FromWin32Boolean(
        _In_ BOOLEAN Value)
    {
        // https://docs.microsoft.com/en-us/windows/win32/winprog/windows-data-types
        this->FromCppBool(Value == TRUE);
    }

    void FromHResult(
        _In_ HRESULT Value)
    {
        if (HRESULT_FACILITY(Value) == FACILITY_WIN32)
        {
            this->m_Value = HRESULT_CODE(Value);
        }
        else
        {
            this->m_Value = ERROR_INVALID_PARAMETER;
        }
    }

    DWORD ToWin32Error()
    {
        return this->m_Value;
    }

    void ToLastError()
    {
        ::SetLastError(this->m_Value);
    }

    bool ToCppBool()
    {
        return (this->m_Value == ERROR_SUCCESS);
    }

    BOOL ToWin32Bool()
    {
        // https://docs.microsoft.com/en-us/windows/win32/winprog/windows-data-types
        return this->ToCppBool() ? TRUE : FALSE;
    }

    BOOLEAN ToWin32Boolean()
    {
        // https://docs.microsoft.com/en-us/windows/win32/winprog/windows-data-types
        return this->ToCppBool() ? TRUE : FALSE;
    }

    HRESULT ToHResult()
    {
        return __HRESULT_FROM_WIN32(this->m_Value);
    }

    CWin32Error() = default;

    CWin32Error(
        _In_ DWORD Value)
    {
        this->FromWin32Error(Value);
    }
};




#include <stdlib.h>
#include <string>

std::wstring GetMessageByID(HRESULT MessageID)
{
    std::wstring MessageString;
    LPWSTR pBuffer = nullptr;

    if (FormatMessageW(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
        nullptr,
        MessageID,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        reinterpret_cast<LPTSTR>(&pBuffer),
        0,
        nullptr))
    {
        MessageString = std::wstring(pBuffer, wcslen(pBuffer));

        LocalFree(pBuffer);
    }

    return MessageString;
}

/**
 * The definition of the file enumerator handle.
 */
typedef void* M2_FILE_ENUMERATOR_HANDLE;
typedef M2_FILE_ENUMERATOR_HANDLE* PM2_FILE_ENUMERATOR_HANDLE;

/**
 * The information about a found file or directory queried from the file
 * enumerator.
 */
typedef struct _M2_FILE_ENUMERATOR_INFORMATION
{
    FILETIME CreationTime;
    FILETIME LastAccessTime;
    FILETIME LastWriteTime;
    FILETIME ChangeTime;
    UINT64 FileSize;
    UINT64 AllocationSize;
    DWORD FileAttributes;
    DWORD EaSize;
    LARGE_INTEGER FileId;
    LPCWSTR ShortName;
    LPCWSTR FileName;
} M2_FILE_ENUMERATOR_INFORMATION, * PM2_FILE_ENUMERATOR_INFORMATION;

/**
 * The internal content of the file enumerator handle.
 */
typedef struct _M2_FILE_ENUMERATOR_OBJECT
{
    HANDLE FileHandle;
    WCHAR ShortNameEnd;
    WCHAR FileNameEnd;
    DWORD Reserved;
    WCHAR* ShortNameEndAddress;
    WCHAR* FileNameEndAddress;
    CRITICAL_SECTION CriticalSection;
    PFILE_ID_BOTH_DIR_INFO CurrentFileInfo;
    BYTE FileInfoBuffer[32768];
} M2_FILE_ENUMERATOR_OBJECT, * PM2_FILE_ENUMERATOR_OBJECT;

const size_t x = sizeof(M2_FILE_ENUMERATOR_OBJECT);

/**
 * Creates a file enumerator handle for searching a directory for a file or
 * subdirectory with a name.
 *
 * @param FileEnumeratorHandle The file enumerator handle.
 * @param FileHandle The handle of the file to be searched a directory for a
 *                   file or subdirectory with a name. This handle must be
 *                   opened with the appropriate permissions for the requested
 *                   change. This handle should not be a pipe handle.
 * @return Standard Win32 Error. If the function succeeds, the return value is
 *         ERROR_SUCCESS.
 * @remark The way to get a file handle for this operation:
 *         HANDLE hFile = CreateFileW(
 *             lpFileName,
 *             FILE_LIST_DIRECTORY | SYNCHRONIZE,
 *             FILE_SHARE_READ | FILE_SHARE_WRITE,
 *             nullptr,
 *             OPEN_EXISTING,
 *             FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OPEN_REPARSE_POINT,
 *             nullptr);
 */
DWORD M2CreateFileEnumerator(
    _Out_ PM2_FILE_ENUMERATOR_HANDLE FileEnumeratorHandle,
    _In_ HANDLE FileHandle)
{
    if (!(FileEnumeratorHandle && (INVALID_HANDLE_VALUE != FileHandle)))
        return ERROR_INVALID_PARAMETER;

    *FileEnumeratorHandle = nullptr;

    PM2_FILE_ENUMERATOR_OBJECT Object =
        reinterpret_cast<PM2_FILE_ENUMERATOR_OBJECT>(HeapAlloc(
            GetProcessHeap(),
            HEAP_ZERO_MEMORY,
            sizeof(M2_FILE_ENUMERATOR_OBJECT)));
    if (!Object)
        return ERROR_NOT_ENOUGH_MEMORY;

    Object->FileHandle = FileHandle;
    ::InitializeCriticalSection(&Object->CriticalSection);

    *FileEnumeratorHandle = Object;

    return ERROR_SUCCESS;
}

/**
 * Closes a created file enumerator handle.
 *
 * @param FileEnumeratorHandle The created file enumerator handle.
 * @return Standard Win32 Error. If the function succeeds, the return value is
 *         ERROR_SUCCESS.
 */
DWORD M2CloseFileEnumerator(
    _In_ M2_FILE_ENUMERATOR_HANDLE FileEnumeratorHandle)
{
    if (!FileEnumeratorHandle)
        return ERROR_INVALID_PARAMETER;

    PM2_FILE_ENUMERATOR_OBJECT Object =
        reinterpret_cast<PM2_FILE_ENUMERATOR_OBJECT>(FileEnumeratorHandle);

    ::DeleteCriticalSection(&Object->CriticalSection);
    ::HeapFree(GetProcessHeap(), 0, Object);

    return ERROR_SUCCESS;
}

/**
 * Starts or continues a file search from a created file enumerator handle.
 *
 * @param FileEnumeratorInformation A pointer to the
 *                                  M2_FILE_ENUMERATOR_INFORMATION structure
 *                                  that receives information about a found
 *                                  file or directory.
 * @param FileEnumeratorHandle The created file enumerator handle.
 * @return Standard Win32 Error. If the function succeeds, the return value is
 *         ERROR_SUCCESS.
 */
DWORD M2QueryFileEnumerator(
    _Out_ PM2_FILE_ENUMERATOR_INFORMATION FileEnumeratorInformation,
    _In_ M2_FILE_ENUMERATOR_HANDLE FileEnumeratorHandle)
{
    if ((!FileEnumeratorHandle) || (!FileEnumeratorInformation))
        return ERROR_INVALID_PARAMETER;

    DWORD Win32Error = ERROR_SUCCESS;

    PM2_FILE_ENUMERATOR_OBJECT Object =
        reinterpret_cast<PM2_FILE_ENUMERATOR_OBJECT>(FileEnumeratorHandle);

    ::EnterCriticalSection(&Object->CriticalSection);

    if (!Object->CurrentFileInfo)
    {
        Object->CurrentFileInfo =
            reinterpret_cast<PFILE_ID_BOTH_DIR_INFO>(Object->FileInfoBuffer);

        if (!::GetFileInformationByHandleEx(
            Object->FileHandle,
            FILE_INFO_BY_HANDLE_CLASS::FileIdBothDirectoryRestartInfo,
            Object->CurrentFileInfo,
            sizeof(Object->FileInfoBuffer)))
        {
            Win32Error = ::GetLastError();
        }
    }
    else if (!Object->CurrentFileInfo->NextEntryOffset)
    {
        Object->CurrentFileInfo =
            reinterpret_cast<PFILE_ID_BOTH_DIR_INFO>(Object->FileInfoBuffer);

        if (!::GetFileInformationByHandleEx(
            Object->FileHandle,
            FILE_INFO_BY_HANDLE_CLASS::FileIdBothDirectoryInfo,
            Object->CurrentFileInfo,
            sizeof(Object->FileInfoBuffer)))
        {
            Win32Error = ::GetLastError();
        }
    }
    else
    {
        *Object->ShortNameEndAddress = Object->ShortNameEnd;
        *Object->FileNameEndAddress = Object->FileNameEnd;

        Object->CurrentFileInfo = reinterpret_cast<PFILE_ID_BOTH_DIR_INFO>(
            reinterpret_cast<ULONG_PTR>(Object->CurrentFileInfo)
            + Object->CurrentFileInfo->NextEntryOffset);
    }

    if (Win32Error == ERROR_SUCCESS)
    {
        PFILE_ID_BOTH_DIR_INFO CurrentFileInfo = Object->CurrentFileInfo;

        FileEnumeratorInformation->CreationTime.dwLowDateTime =
            CurrentFileInfo->CreationTime.LowPart;
        FileEnumeratorInformation->CreationTime.dwHighDateTime =
            CurrentFileInfo->CreationTime.HighPart;
        
        FileEnumeratorInformation->LastAccessTime.dwLowDateTime =
            CurrentFileInfo->LastAccessTime.LowPart;
        FileEnumeratorInformation->LastAccessTime.dwHighDateTime =
            CurrentFileInfo->LastAccessTime.HighPart;
        
        FileEnumeratorInformation->LastWriteTime.dwLowDateTime =
            CurrentFileInfo->LastWriteTime.LowPart;
        FileEnumeratorInformation->LastWriteTime.dwHighDateTime =
            CurrentFileInfo->LastWriteTime.HighPart;
        
        FileEnumeratorInformation->ChangeTime.dwLowDateTime =
            CurrentFileInfo->ChangeTime.LowPart;
        FileEnumeratorInformation->ChangeTime.dwHighDateTime =
            CurrentFileInfo->ChangeTime.HighPart;

        FileEnumeratorInformation->FileSize =
            CurrentFileInfo->EndOfFile.QuadPart;

        FileEnumeratorInformation->AllocationSize =
            CurrentFileInfo->AllocationSize.QuadPart;

        FileEnumeratorInformation->FileAttributes =
            CurrentFileInfo->FileAttributes;

        FileEnumeratorInformation->EaSize =
            CurrentFileInfo->EaSize;

        FileEnumeratorInformation->FileId =
            CurrentFileInfo->FileId;

        Object->ShortNameEndAddress =
            CurrentFileInfo->ShortName +
            (CurrentFileInfo->ShortNameLength / sizeof(WCHAR));
        Object->ShortNameEnd = *Object->ShortNameEndAddress;
        *Object->ShortNameEndAddress = L'\0';
        FileEnumeratorInformation->ShortName =
            CurrentFileInfo->ShortName;

        Object->FileNameEndAddress =
            CurrentFileInfo->FileName +
            (CurrentFileInfo->FileNameLength / sizeof(WCHAR));
        Object->FileNameEnd = *Object->FileNameEndAddress;
        *Object->FileNameEndAddress = L'\0';
        FileEnumeratorInformation->FileName =
            CurrentFileInfo->FileName;
    }

    ::LeaveCriticalSection(&Object->CriticalSection);

    return Win32Error;
}

DWORD M2CreateFile(
    _Out_ PHANDLE lpFileHandle,
    _In_ LPCWSTR lpFileName,
    _In_ DWORD dwDesiredAccess,
    _In_ DWORD dwShareMode,
    _In_opt_ LPSECURITY_ATTRIBUTES lpSecurityAttributes,
    _In_ DWORD dwCreationDisposition,
    _In_ DWORD dwFlagsAndAttributes,
    _In_opt_ HANDLE hTemplateFile)
{
    *lpFileHandle = ::CreateFileW(
        lpFileName,
        dwDesiredAccess,
        dwShareMode,
        lpSecurityAttributes,
        dwCreationDisposition,
        dwFlagsAndAttributes,
        hTemplateFile);

    return (INVALID_HANDLE_VALUE != *lpFileHandle)
        ? ERROR_SUCCESS
        : ::GetLastError();
}

#define WIN32_SUCCESS(Error) (Error == ERROR_SUCCESS)
#define WIN32_ERROR(Status) (Error != ERROR_SUCCESS)

// 判断目录是否是"."或".."
FORCEINLINE bool WINAPI M2IsDots(
    _In_ LPCWSTR Name)
{
    // 由于字符可以看作无符号整数,所以L'\0'即0可看作false
    return Name[0] == L'.' && (!Name[1] || (Name[1] == L'.' && !Name[2]));
}

HRESULT M2BrowseDirectory(
    _In_ LPCWSTR lpFileName)
{
    const size_t MaxPathBufferLength = 1024;
    wchar_t PathBuffer[MaxPathBufferLength];
    size_t PathLength = wcslen(lpFileName);
    wcscpy_s(PathBuffer, MaxPathBufferLength, lpFileName);

    size_t count = 0;

    HANDLE hFile = INVALID_HANDLE_VALUE;
    HRESULT hr = M2CreateFile(
        &hFile,
        lpFileName,
        SYNCHRONIZE | FILE_LIST_DIRECTORY,
        FILE_SHARE_READ | FILE_SHARE_WRITE,
        nullptr,
        OPEN_EXISTING,
        FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OPEN_REPARSE_POINT,
        nullptr);
    if (WIN32_SUCCESS(hr))
    {
        M2_FILE_ENUMERATOR_HANDLE FileEnumeratorHandle = nullptr;
        hr = M2CreateFileEnumerator(&FileEnumeratorHandle, hFile);
        if (WIN32_SUCCESS(hr))
        {
            M2_FILE_ENUMERATOR_INFORMATION FileEnumeratorInformation = { 0 };
            while (WIN32_SUCCESS(M2QueryFileEnumerator(
                &FileEnumeratorInformation,
                FileEnumeratorHandle)))
            {
                if (M2IsDots(FileEnumeratorInformation.FileName))
                    continue;

                wcscpy_s(PathBuffer + PathLength, MaxPathBufferLength - PathLength, FileEnumeratorInformation.FileName);

                //wprintf(L"%s\n", PathBuffer);

                if ((FileEnumeratorInformation.FileAttributes & FILE_ATTRIBUTE_DIRECTORY) &&
                    !(FileEnumeratorInformation.FileAttributes & FILE_ATTRIBUTE_REPARSE_POINT))
                {
                    wcscat_s(PathBuffer + PathLength, MaxPathBufferLength - PathLength, L"\\");

                    M2BrowseDirectory(PathBuffer);
                    continue;
                }

                ++count;

                
            }

            M2CloseFileEnumerator(FileEnumeratorHandle);
        }

        ::CloseHandle(hFile);
    }

    return hr;
}

HRESULT M2BrowseDirectory2(
    _In_ LPCWSTR lpFileName)
{
    size_t count = 0;

    WIN32_FIND_DATAW FindData;
    HANDLE hFind = FindFirstFileW((std::wstring(lpFileName) + L"*").c_str(), &FindData);
    if (INVALID_HANDLE_VALUE != hFind)
    {
        do
        {
            if (M2IsDots(FindData.cFileName))
                continue;

            std::wstring CurrentFileName =
                std::wstring(lpFileName) + FindData.cFileName + L"\\";

            if ((FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) &&
                !(FindData.dwFileAttributes & FILE_ATTRIBUTE_REPARSE_POINT))
            {
                M2BrowseDirectory2(CurrentFileName.c_str());
                continue;
            }

            ++count;

            //wprintf(L"%s\n", CurrentFileName.c_str());

        } while (FindNextFileW(hFind, &FindData));

        //wprintf(L"%s\n", lpFileName);

        FindClose(hFind);
    }

    return S_OK;
}

int main()
{
    //M2BrowseDirectory(L"C:\\Windows\\WinSxS\\Manifests\\");

    ULONGLONG CheckPoint1 = GetTickCount();

    for (size_t i = 0; i < 10; ++i)
    {
        M2BrowseDirectory(L"C:\\");
    }

    ULONGLONG CheckPoint2 = GetTickCount();

    wprintf(L"Time: %lld\n", CheckPoint2 - CheckPoint1);

    getchar();

	return 0;
}
