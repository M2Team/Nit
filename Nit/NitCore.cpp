/*
 * PROJECT:   Nit - A Windows Image Tweaker based on DismCore
 * FILE:      NitCore.cpp
 * PURPOSE:   Implementation for Nit Core Library
 *
 * LICENSE:   The MIT License
 *
 * DEVELOPER: Mouri_Naruto (Mouri_Naruto AT Outlook.com)
 */

#include "NitCore.h"

namespace Nit
{
    /**
     * The information about the Windows Overlay Filter file provider.
     */
    typedef struct _WOF_FILE_PROVIDER_EXTERNAL_INFO
    {
        WOF_EXTERNAL_INFO Wof;
        FILE_PROVIDER_EXTERNAL_INFO FileProvider;
    } WOF_FILE_PROVIDER_EXTERNAL_INFO, *PWOF_FILE_PROVIDER_EXTERNAL_INFO;

    /**
     * @remark You can read the definition for this function in "NitCore.h".
     */
    DWORD WofRemoveFileCompressionAttribute(
        _In_ HANDLE FileHandle)
    {
        if (::DeviceIoControl(
            FileHandle,
            FSCTL_DELETE_EXTERNAL_BACKING,
            nullptr,
            0,
            nullptr,
            0,
            nullptr,
            nullptr))
        {
            return ERROR_SUCCESS;
        }
        else
        {
            return ::GetLastError();
        }
    }

    /**
     * @remark You can read the definition for this function in "NitCore.h".
     */
    DWORD WofSetFileCompressionAttribute(
        _In_ HANDLE FileHandle,
        _In_ DWORD CompressionAlgorithm)
    {
        switch (CompressionAlgorithm)
        {
        case FILE_PROVIDER_COMPRESSION_XPRESS4K:
        case FILE_PROVIDER_COMPRESSION_LZX:
        case FILE_PROVIDER_COMPRESSION_XPRESS8K:
        case FILE_PROVIDER_COMPRESSION_XPRESS16K:
            break;
        default:
            return ERROR_INVALID_PARAMETER;
        }

        WOF_FILE_PROVIDER_EXTERNAL_INFO WofInfo = { 0 };

        WofInfo.Wof.Version = WOF_CURRENT_VERSION;
        WofInfo.Wof.Provider = WOF_PROVIDER_FILE;

        WofInfo.FileProvider.Version = FILE_PROVIDER_CURRENT_VERSION;
        WofInfo.FileProvider.Flags = 0;
        WofInfo.FileProvider.Algorithm = CompressionAlgorithm;

        if (::DeviceIoControl(
            FileHandle,
            FSCTL_SET_EXTERNAL_BACKING,
            &WofInfo,
            sizeof(WofInfo),
            nullptr,
            0,
            nullptr,
            nullptr))
        {
            return ERROR_SUCCESS;
        }
        else
        {
            return ::GetLastError();
        }
    }

    /**
     * @remark You can read the definition for this function in "NitCore.h".
     */
    DWORD WofGetFileCompressionAttribute(
        _Out_ PDWORD CompressionAlgorithm,
        _In_ HANDLE FileHandle)
    {
        if (!CompressionAlgorithm)
            return ERROR_INVALID_PARAMETER;

        WOF_FILE_PROVIDER_EXTERNAL_INFO WofInfo = { 0 };

        if (::DeviceIoControl(
            FileHandle,
            FSCTL_GET_EXTERNAL_BACKING,
            nullptr,
            0,
            &WofInfo,
            sizeof(WofInfo),
            nullptr,
            nullptr))
        {
            *CompressionAlgorithm = WofInfo.FileProvider.Algorithm;
            return ERROR_SUCCESS;
        }
        else
        {
            return ::GetLastError();
        }
    }
}
