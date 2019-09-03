/*
 * PROJECT:   Nit - A Windows Image Tweaker based on DismCore
 * FILE:      NitCore.h
 * PURPOSE:   Definition for Nit Core Library
 *
 * LICENSE:   The MIT License
 *
 * DEVELOPER: Mouri_Naruto (Mouri_Naruto AT Outlook.com)
 */

#ifndef NIT_CORE
#define NIT_CORE

#ifndef __cplusplus
#error "[NitCore] You should use a C++ compiler."
#endif

#include <Windows.h>

namespace Nit
{
    /**
     * Removes the Windows Overlay Filter file compression attribute.
     *
     * @param FileHandle A handle to the file on which the operation is to be
     *        performed.
     * @return Standard Win32 Error. If the function succeeds, the return value is
     *         ERROR_SUCCESS.
     * @remark The way to get a file handle for this operation:
     *         HANDLE FileHandle = CreateFileW(
     *             lpFileName,
     *             FILE_READ_DATA | FILE_READ_ATTRIBUTES,
     *             FILE_SHARE_READ | FILE_SHARE_DELETE,
     *             nullptr,
     *             OPEN_EXISTING,
     *             FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_SEQUENTIAL_SCAN,
     *             nullptr);
     */
    DWORD WofRemoveFileCompressionAttribute(
        _In_ HANDLE FileHandle);

    /**
     * Sets the Windows Overlay Filter file compression attribute.
     *
     * @param FileHandle A handle to the file on which the operation is to be
     *        performed.
     * @param CompressionAlgorithm Specifies the compression algorithm that is
     *        used to compress this file. Currently defined algorithms are:
     *        FILE_PROVIDER_COMPRESSION_XPRESS4K: Indicates that the data for
     *            the file should be compressed in 4kb chunks with the XPress
     *            algorithm. This algorithm is designed to be computationally
     *            lightweight, and provides for rapid access to data.
     *        FILE_PROVIDER_COMPRESSION_LZX: Indicates that the data for the
     *            file should be compressed in 32kb chunks with the LZX
     *            algorithm. This algorithm is designed to be highly compact,
     *            and provides for small footprint for infrequently accessed
     *            data.
     *        FILE_PROVIDER_COMPRESSION_XPRESS8K: Indicates that the data for
     *            the file should be compressed in 8kb chunks with the XPress
     *            algorithm.
     *        FILE_PROVIDER_COMPRESSION_XPRESS16K: Indicates that the data for
     *            the file should be compressed in 16kb chunks with the XPress
     *            algorithm.
     * @return Standard Win32 Error. If the function succeeds, the return value is
     *         ERROR_SUCCESS.
     * @remark The way to get a file handle for this operation:
     *         HANDLE FileHandle = CreateFileW(
     *             lpFileName,
     *             FILE_READ_DATA | FILE_READ_ATTRIBUTES,
     *             FILE_SHARE_READ | FILE_SHARE_DELETE,
     *             nullptr,
     *             OPEN_EXISTING,
     *             FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_SEQUENTIAL_SCAN,
     *             nullptr);
     */
    DWORD WofSetFileCompressionAttribute(
        _In_ HANDLE FileHandle,
        _In_ DWORD CompressionAlgorithm);

    /**
     * Gets the Windows Overlay Filter file compression attribute.
     *
     * @param CompressionAlgorithm A pointer to the compression algorithm that
     *        is used to compress this file. Currently defined algorithms are:
     *        FILE_PROVIDER_COMPRESSION_XPRESS4K: Indicates that the data for
     *            the file should be compressed in 4kb chunks with the XPress
     *            algorithm. This algorithm is designed to be computationally
     *            lightweight, and provides for rapid access to data.
     *        FILE_PROVIDER_COMPRESSION_LZX: Indicates that the data for the
     *            file should be compressed in 32kb chunks with the LZX
     *            algorithm. This algorithm is designed to be highly compact,
     *            and provides for small footprint for infrequently accessed
     *            data.
     *        FILE_PROVIDER_COMPRESSION_XPRESS8K: Indicates that the data for
     *            the file should be compressed in 8kb chunks with the XPress
     *            algorithm.
     *        FILE_PROVIDER_COMPRESSION_XPRESS16K: Indicates that the data for
     *            the file should be compressed in 16kb chunks with the XPress
     *            algorithm.
     * @param FileHandle A handle to the file on which the operation is to be
     *        performed.
     * @return Standard Win32 Error. If the function succeeds, the return value is
     *         ERROR_SUCCESS.
     * @remark The way to get a file handle for this operation:
     *         HANDLE FileHandle = CreateFileW(
     *             lpFileName,
     *             FILE_READ_DATA | FILE_READ_ATTRIBUTES,
     *             FILE_SHARE_READ | FILE_SHARE_DELETE,
     *             nullptr,
     *             OPEN_EXISTING,
     *             FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_SEQUENTIAL_SCAN,
     *             nullptr);
     */
    DWORD WofGetFileCompressionAttribute(
        _Out_ PDWORD CompressionAlgorithm,
        _In_ HANDLE FileHandle);

    /**
     * Sets the NTFS file compression attribute.
     *
     * @param FileHandle A handle to the file on which the operation is to be
     *        performed.
     * @param CompressionAlgorithm Specifies the compression algorithm that is
     *        used to compress this file. Currently defined algorithms are:
     *        COMPRESSION_FORMAT_NONE: Uncompress the file or directory.
     *        COMPRESSION_FORMAT_DEFAULT: Compress the file or directory, using
     *            the default compression format.
     *        COMPRESSION_FORMAT_LZNT1: Compress the file or directory, using
     *            the LZNT1 compression format.
     * @return Standard Win32 Error. If the function succeeds, the return value is
     *         ERROR_SUCCESS.
     * @remark The way to get a file handle for this operation:
     *         HANDLE FileHandle = CreateFileW(
     *             lpFileName,
     *             FILE_READ_DATA | FILE_WRITE_DATA,
     *             FILE_SHARE_READ | FILE_SHARE_WRITE,
     *             nullptr,
     *             OPEN_EXISTING,
     *             FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_SEQUENTIAL_SCAN,
     *             nullptr);
     */
    DWORD NtfsSetFileCompressionAttribute(
        _In_ HANDLE FileHandle,
        _In_ USHORT CompressionAlgorithm);

    /**
     * Gets the NTFS file compression attribute.
     *
     * @param CompressionAlgorithm A pointer to the compression algorithm that
     *        is used to compress this file. Currently defined algorithms are:
     *        COMPRESSION_FORMAT_NONE: The file or directory is not compressed.
     *        COMPRESSION_FORMAT_LZNT1: The file or directory is compressed,
     *            using the LZNT1 compression format.
     * @param FileHandle A handle to the file on which the operation is to be
     *        performed.
     * @return Standard Win32 Error. If the function succeeds, the return value is
     *         ERROR_SUCCESS.
     * @remark The way to get a file handle for this operation:
     *         HANDLE FileHandle = CreateFileW(
     *             lpFileName,
     *             FILE_READ_DATA | FILE_WRITE_DATA,
     *             FILE_SHARE_READ | FILE_SHARE_WRITE,
     *             nullptr,
     *             OPEN_EXISTING,
     *             FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_SEQUENTIAL_SCAN,
     *             nullptr);
     */
    DWORD NtfsGetFileCompressionAttribute(
        _Out_ PUSHORT CompressionAlgorithm,
        _In_ HANDLE FileHandle);
}

#endif
