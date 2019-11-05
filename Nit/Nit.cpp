/*
 * PROJECT:   Nit - A Windows Image Tweaker based on DismCore
 * FILE:      Nit.cpp
 * PURPOSE:   Implementation for Nit
 *
 * LICENSE:   The MIT License
 *
 * DEVELOPER: Mouri_Naruto (Mouri_Naruto AT Outlook.com)
 */

#define NOMINMAX

#include <Windows.h>

#include <cstdint>

#include <stdlib.h>
#include <string>

#include <strsafe.h>

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
    WCHAR ShortName[14];
    WCHAR FileName[256];
} M2_FILE_ENUMERATOR_INFORMATION, * PM2_FILE_ENUMERATOR_INFORMATION;

/**
 * The internal content of the file enumerator handle.
 */
typedef struct _M2_FILE_ENUMERATOR_OBJECT
{
    HANDLE FileHandle;
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
        reinterpret_cast<PM2_FILE_ENUMERATOR_OBJECT>(::HeapAlloc(
            ::GetProcessHeap(),
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
    ::HeapFree(::GetProcessHeap(), 0, Object);

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

    DWORD ErrorCode = ERROR_SUCCESS;

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
            ErrorCode = ::GetLastError();
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
            ErrorCode = ::GetLastError();
        }
    }
    else
    {
        Object->CurrentFileInfo = reinterpret_cast<PFILE_ID_BOTH_DIR_INFO>(
            reinterpret_cast<ULONG_PTR>(Object->CurrentFileInfo)
            + Object->CurrentFileInfo->NextEntryOffset);
    }

    if (ErrorCode == ERROR_SUCCESS)
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

        ::StringCbCopyNW(
            FileEnumeratorInformation->ShortName,
            sizeof(FileEnumeratorInformation->ShortName),
            CurrentFileInfo->ShortName,
            CurrentFileInfo->ShortNameLength);

        ::StringCbCopyNW(
            FileEnumeratorInformation->FileName,
            sizeof(FileEnumeratorInformation->FileName),
            CurrentFileInfo->FileName,
            CurrentFileInfo->FileNameLength);

        /*::wcsncpy_s(
            FileEnumeratorInformation->ShortName,
            sizeof(FileEnumeratorInformation->ShortName) / sizeof(WCHAR),
            CurrentFileInfo->ShortName,
            CurrentFileInfo->ShortNameLength / sizeof(WCHAR));

        ::wcsncpy_s(
            FileEnumeratorInformation->FileName,
            sizeof(FileEnumeratorInformation->FileName) / sizeof(WCHAR),
            CurrentFileInfo->FileName,
            CurrentFileInfo->FileNameLength / sizeof(WCHAR));*/
    }

    ::LeaveCriticalSection(&Object->CriticalSection);

    return ErrorCode;
}

// 判断目录是否是"."或".."
FORCEINLINE bool WINAPI M2IsDots(
    _In_ LPCWSTR Name)
{
    // 由于字符可以看作无符号整数,所以L'\0'即0可看作false
    return Name[0] == L'.' && (!Name[1] || (Name[1] == L'.' && !Name[2]));
}

#include <algorithm>

DWORD GetFileCompressedSize(
    _Out_ PUINT64 CompressedFileSize,
    _In_ HANDLE FileHandle)
{
    FILE_COMPRESSION_INFO FileCompressionInfo = { 0 };
    if (::GetFileInformationByHandleEx(
        FileHandle,
        FILE_INFO_BY_HANDLE_CLASS::FileCompressionInfo,
        &FileCompressionInfo,
        sizeof(FILE_COMPRESSION_INFO)))
    {
        *CompressedFileSize = FileCompressionInfo.CompressedFileSize.QuadPart;
        return ERROR_SUCCESS;
    }
    else
    {
        return ::GetLastError();
    }
}

DWORD GetFileAllocationSize(
    _Out_ PUINT64 AllocationSize,
    _In_ HANDLE FileHandle)
{
    if (!AllocationSize)
        return ERROR_INVALID_PARAMETER;



    FILE_STANDARD_INFO FileStandardInfo = { 0 };
    if (!::GetFileInformationByHandleEx(
        FileHandle,
        FILE_INFO_BY_HANDLE_CLASS::FileStandardInfo,
        &FileStandardInfo,
        sizeof(FILE_STANDARD_INFO)))
    {
        return ::GetLastError();
    }

    FILE_COMPRESSION_INFO FileCompressionInfo = { 0 };
    if (!::GetFileInformationByHandleEx(
        FileHandle,
        FILE_INFO_BY_HANDLE_CLASS::FileCompressionInfo,
        &FileCompressionInfo,
        sizeof(FILE_COMPRESSION_INFO)))
    {
        return ::GetLastError();
    }

    *AllocationSize = std::max(FileStandardInfo.EndOfFile.QuadPart, FileCompressionInfo.CompressedFileSize.QuadPart);


    return ERROR_SUCCESS;
}

#include <functional>

enum LoopType
{
    Normal,
    Break,
    Continue
};

LoopType EnumerateDirectoryCallbackType(
    std::wstring const& FilePath,
    M2_FILE_ENUMERATOR_INFORMATION const& FileInformation);

void EnumerateDirectory(
    std::wstring const& DirectoryPath,
    std::function<decltype(EnumerateDirectoryCallbackType)> const& CallBack)
{
    HANDLE hDirectory = ::CreateFileW(
        (std::wstring(L"\\\\?\\") + DirectoryPath).c_str(),
        SYNCHRONIZE | FILE_LIST_DIRECTORY,
        FILE_SHARE_READ | FILE_SHARE_WRITE,
        nullptr,
        OPEN_EXISTING,
        FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OPEN_REPARSE_POINT,
        nullptr);

    if (INVALID_HANDLE_VALUE == hDirectory)
    {
        std::wprintf(
            L"%s(%s) failed with error code %d\n",
            L"CreateFileW",
            DirectoryPath.c_str(),
            ::GetLastError());
    }
    else
    {
        M2_FILE_ENUMERATOR_HANDLE FileEnumeratorHandle = nullptr;

        DWORD Status = M2CreateFileEnumerator(
            &FileEnumeratorHandle,
            hDirectory);
        if (Status != ERROR_SUCCESS)
        {
            std::wprintf(
                L"%s(%s) failed with error code %d\n",
                L"M2CreateFileEnumerator",
                DirectoryPath.c_str(),
                Status);
        }
        else
        {
            M2_FILE_ENUMERATOR_INFORMATION FileEnumeratorInformation = { 0 };
            while (ERROR_SUCCESS == M2QueryFileEnumerator(
                &FileEnumeratorInformation,
                FileEnumeratorHandle))
            {
                if (M2IsDots(FileEnumeratorInformation.FileName))
                    continue;

                std::wstring FilePath = DirectoryPath + FileEnumeratorInformation.FileName;
                if (FileEnumeratorInformation.FileAttributes & FILE_ATTRIBUTE_DIRECTORY)
                    FilePath += L"\\";

                LoopType ReturnType = CallBack(
                    FilePath,
                    FileEnumeratorInformation);
                if (ReturnType == LoopType::Continue)
                    continue;
                if (ReturnType == LoopType::Break)
                    break;

                if (FileEnumeratorInformation.FileAttributes & FILE_ATTRIBUTE_DIRECTORY)
                {
                    EnumerateDirectory(FilePath.c_str(), CallBack);
                }
            }

            M2CloseFileEnumerator(FileEnumeratorHandle);
        }

        ::CloseHandle(hDirectory);
    }
}

#include <cwchar>
#include <cwctype>

#include <clocale>

#include <set>

#include <map>

#include "NitCore.h"

#include <NSudoAPI.h>

#include "NitVersion.h"

DWORD NitMatchFileName(
    _In_ LPCWSTR FilePath,
    _In_ SIZE_T FilePathLength,
    _In_ LPCWSTR Pattern,
    _In_ SIZE_T PatternLength)
{
    SIZE_T i = 0;
    SIZE_T j = 0;
    SIZE_T start = static_cast<SIZE_T>(-1);
    SIZE_T match = 0;
    while (i < FilePathLength)
    {
        if (j < PatternLength)
        {
            if (std::towlower(FilePath[i]) == std::towlower(Pattern[j]) ||
                Pattern[j] == L'?')
            {
                ++i;
                ++j;
                continue;
            }
            else if (Pattern[j] == L'*')
            {
                start = j;
                match = i;
                ++j;
                continue;
            }
        }

        if (start != static_cast<SIZE_T>(-1))
        {
            j = start + 1;
            ++match;
            i = match;
            continue;
        }
        else
        {
            return ERROR_NOT_FOUND;
        }
    }

    while (j < PatternLength)
    {
        if (Pattern[j] != L'*')
        {
            return ERROR_NOT_FOUND;
        }

        ++j;
    }

    return ERROR_SUCCESS;
}

int main()
{
    std::setlocale(LC_ALL, "chs");

    std::wprintf(
        L"Mouri_Naruto Nit [Version " NIT_VERSION_STRING L"]\n"
        L"(C) M2-Team and Contributors. All rights reserved.\n"
        L"\n");

    INSudoClient* pNSudoClient = nullptr;
    HRESULT hr = ::NSudoCreateInstance(
        IID_INSudoClient, reinterpret_cast<PVOID*>(&pNSudoClient));
    if (hr == S_OK)
    {
        HANDLE CurrentProcessToken = nullptr;
        hr = pNSudoClient->OpenCurrentProcessToken(
            MAXIMUM_ALLOWED, &CurrentProcessToken);
        if (hr == S_OK)
        {
            std::map<std::wstring, DWORD> Privileges;

            Privileges.insert(std::pair(
                SE_BACKUP_NAME, SE_PRIVILEGE_ENABLED));
            Privileges.insert(std::pair(
                SE_RESTORE_NAME, SE_PRIVILEGE_ENABLED));

            std::vector<LUID_AND_ATTRIBUTES> RawPrivileges;

            for (auto const& Privilege : Privileges)
            {
                LUID_AND_ATTRIBUTES RawPrivilege;

                if (!::LookupPrivilegeValueW(
                    nullptr, Privilege.first.c_str(), &RawPrivilege.Luid))
                {
                    hr = ::HRESULT_FROM_WIN32(::GetLastError());
                    break;
                }

                RawPrivilege.Attributes = Privilege.second;

                RawPrivileges.push_back(RawPrivilege);
            }

            if (hr == S_OK)
            {
                hr = pNSudoClient->AdjustTokenPrivileges(
                    CurrentProcessToken,
                    &RawPrivileges[0],
                    static_cast<DWORD>(RawPrivileges.size()));
            }

            ::CloseHandle(CurrentProcessToken);
        }

        pNSudoClient->Release();
    }

    if (hr != S_OK)
    {
        std::wprintf(
            L"%s(%s) failed with error code %d\n",
            L"main::elevate",
            SE_BACKUP_NAME L" and " SE_RESTORE_NAME,
            hr);
        return hr;
    }

    const DWORD CompressionAlgorithm = FILE_PROVIDER_COMPRESSION_XPRESS4K;

    std::set<std::wstring> ExclusionList;

    ExclusionList.insert(L"*\\??ldr");
    ExclusionList.insert(L"*\\BootMgr");

    ExclusionList.insert(L"*\\aow.wim");
    ExclusionList.insert(L"*\\Boot\\BCD*");
    ExclusionList.insert(L"*\\Boot\\bootstat.dat");
    ExclusionList.insert(L"*\\config\\DRIVERS*");
    ExclusionList.insert(L"*\\config\\SYSTEM*");
    ExclusionList.insert(L"*\\Windows\\bootstat.dat");
    ExclusionList.insert(L"*\\winload.e??*");
    ExclusionList.insert(L"*\\winresume.e??*");

    ExclusionList.insert(L"*\\WinSxS\\Backup\\*");
    ExclusionList.insert(L"*\\WinSxS\\Manifest*\\*");

    EnumerateDirectory(
        L"C:\\",
        [&](
            std::wstring const& FilePath,
            M2_FILE_ENUMERATOR_INFORMATION const& FileInformation) -> LoopType
        {
            if (FileInformation.FileAttributes & FILE_ATTRIBUTE_REPARSE_POINT)
                return LoopType::Continue;

            for (auto& ExclusionItem : ExclusionList)
            {
                if (ERROR_SUCCESS == NitMatchFileName(
                    FilePath.c_str(),
                    FilePath.size(),
                    ExclusionItem.c_str(),
                    ExclusionItem.size()))
                {
                    std::wprintf(L"Excluded - [%s]\n", FilePath.c_str());
                    return LoopType::Continue;
                }
            }

            if (FileInformation.FileAttributes & FILE_ATTRIBUTE_DIRECTORY)
                return LoopType::Normal;

            HANDLE FileHandle = ::CreateFileW(
                (std::wstring(L"\\\\?\\") + FilePath).c_str(),
                FILE_READ_DATA | FILE_READ_ATTRIBUTES,
                FILE_SHARE_READ | FILE_SHARE_DELETE,
                nullptr,
                OPEN_EXISTING,
                FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_SEQUENTIAL_SCAN,
                nullptr);
            if (FileHandle != INVALID_HANDLE_VALUE)
            {
                DWORD CurrentCompressionAlgorithm = FILE_PROVIDER_COMPRESSION_XPRESS4K;
                DWORD Status = Nit::WofGetFileCompressionAttribute(
                    &CurrentCompressionAlgorithm,
                    FileHandle);
                if (!(Status == ERROR_SUCCESS
                    && CurrentCompressionAlgorithm == CompressionAlgorithm))
                {
                    Status = Nit::WofSetFileCompressionAttribute(
                        FileHandle,
                        CompressionAlgorithm);
                    if (Status != ERROR_COMPRESSION_NOT_BENEFICIAL)
                    {
                        if (Status == ERROR_SUCCESS)
                        {
                            std::wprintf(L"Compressed - [%s]\n", FilePath.c_str());
                        }
                        else
                        {
                            std::wprintf(
                                L"%s(%s) failed with error code %d\n",
                                L"Nit::WofSetFileCompressionAttribute",
                                FilePath.c_str(),
                                Status);
                        }
                    }
                }

                ::CloseHandle(FileHandle);
            }
            else
            {
                std::wprintf(
                    L"%s(%s) failed with error code %d\n",
                    L"CreateFileW",
                    FilePath.c_str(),
                    ::GetLastError());
            }

            return LoopType::Normal;
        });

    std::wprintf(L"\n\nFinished.\n");

    getchar();

    return 0;
}








DWORD GetStringValue(
    _In_ HKEY hKey,
    _In_opt_ LPCWSTR lpValueName,
    _Out_opt_ LPDWORD lpType,
    _Out_opt_ LPBYTE lpData,
    _Inout_opt_ LPDWORD lpcbData)
{
    return ::RegQueryValueExW(
        hKey,
        lpValueName,
        nullptr,
        lpType,
        lpData,
        lpcbData);
}

DWORD SetStringValue(
    _In_ HKEY hKey,
    _In_opt_ LPCWSTR lpValueName,
    _In_ LPCWSTR Value)
{
    return ::RegSetValueExW(
        hKey,
        lpValueName,
        0,
        REG_SZ,
        reinterpret_cast<CONST BYTE*>(Value),
        static_cast<DWORD>(wcslen(Value) + 1) * sizeof(wchar_t));
}




















/**
     * Disable C++ Object Copying
     */
class CDisableObjectCopying
{
protected:
    CDisableObjectCopying() = default;
    ~CDisableObjectCopying() = default;

private:
    CDisableObjectCopying(
        const CDisableObjectCopying&) = delete;
    CDisableObjectCopying& operator=(
        const CDisableObjectCopying&) = delete;
};

/**
 * Disable C++ Object Moving
 */
class CDisableObjectMoving
{
protected:
    CDisableObjectMoving() = default;
    ~CDisableObjectMoving() = default;

private:
    CDisableObjectMoving(
        const CDisableObjectCopying&&) = delete;
    CDisableObjectMoving& operator=(
        const CDisableObjectCopying&&) = delete;
};

/**
 * The implementation of smart object.
 */
template<typename TDerive, typename TObject, const TObject InvalidValue>
class CObject : CDisableObjectCopying, CDisableObjectMoving
{
private:

    TObject m_RawObject;

protected:

    TObject RawObject()
    {
        return this->m_RawObject;
    }

    void RawObject(TObject RawObject)
    {
        this->m_RawObject = RawObject;
    }

    TObject* RawObjectPointer()
    {
        return &this->m_RawObject;
    }

public:

    TObject* operator&()
    {
        return this->RawObjectPointer();
    }

    operator TObject()
    {
        return this->RawObject();
    }

    TObject operator->() const
    {
        return this->RawObject();
    }

public:

    CObject(TObject Object = InvalidValue)
    {
        this->RawObject(Object);
    }

    ~CObject()
    {
        this->Close();
    }

    TObject operator=(TObject Object)
    {
        if (Object != this->RawObject())
        {
            this->Close();
            this->RawObject(Object);
        }
        return this->RawObject();
    }

    bool IsInvalid()
    {
        return (this->RawObject() == InvalidValue);
    }

    TObject Detach()
    {
        TObject Object = this->RawObject();
        this->RawObject(InvalidValue);
        return Object;
    }

    void Close()
    {
        if (!this->IsInvalid())
        {
            reinterpret_cast<TDerive*>(this)->CloseObject();
            this->RawObject(InvalidValue);
        }
    }
};

class CRegistryKey : public CObject<CRegistryKey, HKEY, nullptr>
{
public:

    void CloseObject()
    {
        ::RegCloseKey(this->RawObject());
    }

    CRegistryKey(HKEY KeyHandle = nullptr) :
        CObject<CRegistryKey, HKEY, nullptr>(KeyHandle)
    {
    }

    DWORD Get(
        _In_opt_ LPCWSTR ValueName,
        _Out_opt_ LPDWORD Type,
        _Out_opt_ LPBYTE Data,
        _Inout_opt_ LPDWORD DataLength)
    {
        return ::RegQueryValueExW(
            this->RawObject(),
            ValueName,
            nullptr,
            Type,
            Data,
            DataLength);
    }

    DWORD Set(
        _In_opt_ LPCWSTR ValueName,
        _In_ DWORD Type,
        _In_opt_ LPCBYTE Data,
        _In_ DWORD DataLength)
    {
        return ::RegSetValueExW(
            this->RawObject(),
            ValueName,
            0,
            Type,
            Data,
            DataLength);
    }

public:

    DWORD GetDWORD(
        _In_opt_ LPCWSTR ValueName,
        _Out_ PDWORD Value)
    {
        DWORD ErrorCode = ERROR_INVALID_PARAMETER;

        if (Value)
        {
            DWORD Type = REG_DWORD;
            DWORD cbData = static_cast<DWORD>(sizeof(DWORD));

            ErrorCode = this->Get(
                ValueName,
                &Type,
                reinterpret_cast<LPBYTE>(Value),
                &cbData);
            if (Type != REG_DWORD)
            {
                ErrorCode = ERROR_UNSUPPORTED_TYPE;
            }
        }

        return ErrorCode;
    }

    DWORD SetDWORD(
        _In_opt_ LPCWSTR ValueName,
        _In_ DWORD Value)
    {
        return this->Set(
            ValueName,
            REG_DWORD,
            reinterpret_cast<LPCBYTE>(&Value),
            static_cast<DWORD>(sizeof(DWORD)));
    }

    DWORD GetQWORD(
        _In_opt_ LPCWSTR ValueName,
        _Out_ PUINT64 Value)
    {
        DWORD ErrorCode = ERROR_INVALID_PARAMETER;

        if (Value)
        {
            DWORD Type = REG_QWORD;
            DWORD cbData = static_cast<DWORD>(sizeof(UINT64));

            ErrorCode = this->Get(
                ValueName,
                &Type,
                reinterpret_cast<LPBYTE>(Value),
                &cbData);
            if (Type != REG_QWORD)
            {
                ErrorCode = ERROR_UNSUPPORTED_TYPE;
            }
        }

        return ErrorCode;
    }

    DWORD SetQWORD(
        _In_opt_ LPCWSTR ValueName,
        _In_ UINT64 Value)
    {
        return this->Set(
            ValueName,
            REG_QWORD,
            reinterpret_cast<CONST BYTE*>(&Value),
            static_cast<DWORD>(sizeof(UINT64)));
    }
};






DWORD RegCreateKeyWrapper(
    _In_ HKEY ExistingKeyHandle,
    _In_ LPCWSTR SubKey,
    _In_opt_ LPWSTR Class,
    _In_ DWORD CreateOptions,
    _In_ REGSAM DesiredAccess,
    _In_opt_ CONST LPSECURITY_ATTRIBUTES SecurityAttributes,
    _Out_ PHKEY NewKeyHandle,
    _Out_opt_ LPDWORD Disposition)
{
    DWORD ErrorCode = ::RegCreateKeyExW(
        ExistingKeyHandle,
        SubKey,
        0,
        Class,
        CreateOptions | REG_OPTION_BACKUP_RESTORE,
        DesiredAccess,
        SecurityAttributes,
        NewKeyHandle,
        Disposition);
    if (ErrorCode != ERROR_SUCCESS)
    {
        ErrorCode = ::RegCreateKeyExW(
            ExistingKeyHandle,
            SubKey,
            0,
            Class,
            CreateOptions & (-1 ^ REG_OPTION_BACKUP_RESTORE),
            DesiredAccess,
            SecurityAttributes,
            NewKeyHandle,
            Disposition);
    }

    return ErrorCode;
}




int main2()
{
    DWORD ErrorCode = ERROR_INVALID_PARAMETER;

    CRegistryKey SetupKeyObject;
    ErrorCode = ::RegCreateKeyWrapper(
        HKEY_LOCAL_MACHINE,
        L"SYSTEM\\Setup",
        nullptr,
        REG_OPTION_NON_VOLATILE | REG_OPTION_BACKUP_RESTORE,
        KEY_ALL_ACCESS | KEY_WOW64_64KEY,
        nullptr,
        &SetupKeyObject,
        nullptr);
    if (ErrorCode == ERROR_SUCCESS)
    {
        DWORD CompactOSMode = 0;
        ErrorCode = SetupKeyObject.GetDWORD(L"Compact", &CompactOSMode);

        wprintf(L"HKEY_LOCAL_MACHINE\\SYSTEM\\Setup -> Compact = 0x%08X", CompactOSMode);
    }

    ::getchar();

    return 0;
}


// NitSendCallback(SessionHandle,)

//HRESULT NitCleanupHandler(_In_ PVOID SessionHandle, _Inout_ UINT64 Size)
//{
//    // SessionHandle; Callback;
//}
//
//void xyz()
//{
//    bool Cleanup = true;
//
//    if (Cleanup)
//    {
//
//    }
//}

int main1()
{

    HANDLE hDirectory = ::CreateFileW(
        L"\\\\?\\C:\\Windows\\WinSxS\\Manifests",
        SYNCHRONIZE | FILE_LIST_DIRECTORY,
        FILE_SHARE_READ | FILE_SHARE_WRITE,
        nullptr,
        OPEN_EXISTING,
        FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OPEN_REPARSE_POINT,
        nullptr);
    if (INVALID_HANDLE_VALUE != hDirectory)
    {
        M2_FILE_ENUMERATOR_HANDLE FileEnumeratorHandle = nullptr;
        M2_FILE_ENUMERATOR_INFORMATION FileEnumeratorInformation = { 0 };

        DWORD ErrorCode = M2CreateFileEnumerator(&FileEnumeratorHandle, hDirectory);
        if (ErrorCode == ERROR_SUCCESS)
        {
            while (ERROR_SUCCESS == M2QueryFileEnumerator(
                &FileEnumeratorInformation,
                FileEnumeratorHandle))
            {
                wprintf(L"%s\n", FileEnumeratorInformation.FileName);
                //FileList.insert(FileEnumeratorInformation.FileName);
            }

            M2CloseFileEnumerator(FileEnumeratorHandle);
        }

        ::CloseHandle(hDirectory);
    }

    //::getchar();

    return 0;
}

namespace Nit::Platform::Common
{
    /***
*compares count wchar_t of strings,ignore case
*
*Purpose:
*       Compare the two strings for ordinal order.  Stops the comparison
*       when the following occurs: (1) strings differ, (2) the end of the
*       strings is reached, or (3) count characters have been compared.
*       For the purposes of the comparison, upper case characters are
*       converted to lower case (wide-characters).
*
*Entry:
*       wchar_t *lhs, *rhs - strings to compare
*       size_t count - maximum number of characters to compare
*
*Exit:
*       Returns -1 if lhs < rhs
*       Returns 0 if lhs == rhs
*       Returns 1 if lhs > rhs
*       Returns _NLSCMPERROR if something went wrong
*       This range of return values may differ from other *cmp/*coll functions.
*
*Exceptions:
*       Input parameters are validated. Refer to the validation section of the function.
*
*******************************************************************************/
    int wcsnicmp(
        wchar_t const* const lhs,
        wchar_t const* const rhs,
        std::size_t const count)
    {
        if (count == 0)
        {
            return 0;
        }

        std::wint_t const* lhs_ptr = reinterpret_cast<std::wint_t const*>(lhs);
        std::wint_t const* rhs_ptr = reinterpret_cast<std::wint_t const*>(rhs);

        std::wint_t result;
        std::wint_t lhs_value;
        std::wint_t rhs_value;
        std::size_t remaining = count;
        do
        {
            lhs_value = std::towlower(*lhs_ptr++);
            rhs_value = std::towlower(*rhs_ptr++);
            result = lhs_value - rhs_value;
        } while (result == 0 && lhs_value != 0 && --remaining != 0);

        return result;
    }

    /***
*compare wide-character strings, ignore case
*
*Purpose:
*       _wcsicmp performs a case-insensitive wchar_t string comparision.
*       _wcsicmp is independent of locale.
*
*Entry:
*       wchar_t *lhs, *rhs - strings to compare
*
*Return:
*       Returns <0 if lhs < rhs
*       Returns 0 if lhs = rhs
*       Returns >0 if lhs > rhs
*       Returns _NLSCMPERROR if something went wrong
*       This range of return values may differ from other *cmp/*coll functions.
*
*Exceptions:
*       Input parameters are validated. Refer to the validation section of the function.
*
*******************************************************************************/
    int wcsicmp(
        wchar_t const* const lhs,
        wchar_t const* const rhs)
    {
        std::wint_t const* lhs_ptr = reinterpret_cast<std::wint_t const*>(lhs);
        std::wint_t const* rhs_ptr = reinterpret_cast<std::wint_t const*>(rhs);

        std::wint_t result;
        std::wint_t lhs_value;
        std::wint_t rhs_value;
        do
        {
            lhs_value = std::towlower(*lhs_ptr++);
            rhs_value = std::towlower(*rhs_ptr++);
            result = lhs_value - rhs_value;
        } while (result == 0 && lhs_value != 0);

        return result;
    }
}

BOOL StrRegexMatch(LPCWSTR Str, LPCWSTR MatchStr)
{
    LPCWSTR Next;
    //LPCWSTR MatchStr = *this;
    LPCWSTR _Now = Str;
    std::size_t cchNow = std::wcslen(Str);

    std::size_t cchMatch;
    int Flage = 1;

    for (;;)
    {
        Next = std::wcschr(MatchStr, L'*');
        if (!Next)
        {
            break;
        }

        //进行正则*匹配
        cchMatch = Next - MatchStr;

        for (int i = 0; *Str && i != Flage; i++, Str++)
        {
            if (Nit::Platform::Common::wcsnicmp(Str, MatchStr, cchMatch) == 0)
            {
                goto St;
            }
        }

        return FALSE;

    St:
        Flage = -1;
        Str += cchMatch;
        MatchStr = Next + 1;
    }

    if (Flage != 1)
    {
        cchMatch = cchNow - (Str - _Now) - std::wcslen(MatchStr);

        Str = cchMatch >= 0 ? Str + cchMatch : NULL;
    }

    if (Str)
        return Nit::Platform::Common::wcsicmp(Str, MatchStr) == 0;
    else
        return 0;
}



bool wildcard_string_match(
    char const* const string,
    size_t const string_length,
    char const* const pattern,
    size_t const pattern_length)
{
    size_t i = 0;
    size_t j = 0;
    size_t start = static_cast<size_t>(-1);
    size_t match = 0;
    while (i < string_length)
    {
        if (j < pattern_length)
        {
            if (string[i] == pattern[j] || pattern[j] == L'?')
            {
                ++i;
                ++j;
                continue;
            }
            else if (pattern[j] == L'*')
            {
                start = j;
                match = i;
                ++j;
                continue;
            }
        }
        
        if (start != static_cast<size_t>(-1))
        {
            j = start + 1;
            ++match;
            i = match;
            continue;
        }
        else
        {
            return false;
        }
    }

    while (j < pattern_length)
    {
        if (pattern[j] != L'*')
        {
            return false;
        }

        ++j;
    }

    return true;
}

bool isMatch(std::string s, std::string p)
{
    return wildcard_string_match(s.c_str(), s.size(), p.c_str(), p.size());
}

int main3()
{
    isMatch("abefcdgiescdfimde", "ab*cd?i*de");

    return 0;
}
