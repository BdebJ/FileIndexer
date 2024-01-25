#include "indexer.h"

DWORD index_directory(TCHAR directory_path[], int width_padding)
{
    WIN32_FIND_DATA ffd;
    LARGE_INTEGER filesize;
    HANDLE hFind = INVALID_HANDLE_VALUE;
    DWORD dwError = 0;

    TCHAR directory_path_copy[MAX_PATH];
    StringCchCopy(directory_path_copy, MAX_PATH, directory_path);
    // Appending '\*' to the local directory name copy
    StringCchCat(directory_path_copy, MAX_PATH, TEXT("\\*"));

    hFind = FindFirstFile(directory_path_copy, &ffd);

    if (INVALID_HANDLE_VALUE == hFind)
    {
        dwError = GetLastError();
        print_error(TEXT("FindFirstFile"));
        return dwError;
    }

    do
    {
        if (_tcscmp(ffd.cFileName, TEXT(".")) == 0 || _tcscmp(ffd.cFileName, TEXT("..")) == 0)
        {
            continue;
        }
        if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        {
            _tprintf(TEXT("%*s%s   <DIR>\n"), width_padding, TEXT(""), ffd.cFileName);

            // Recurse 
            TCHAR subdirectory_path[MAX_PATH];
            StringCchCopy(subdirectory_path, MAX_PATH, directory_path);
            StringCchCat(subdirectory_path, MAX_PATH, TEXT("\\"));
            StringCchCat(subdirectory_path, MAX_PATH, ffd.cFileName);

            dwError = index_directory(subdirectory_path, width_padding + 4);
            if (dwError != ERROR_SUCCESS)
            {
                return dwError;
            }
        }
        else
        {
            filesize.LowPart = ffd.nFileSizeLow;
            filesize.HighPart = ffd.nFileSizeHigh;
            _tprintf(TEXT("%*s%s   %lld bytes\n"), width_padding, TEXT(""), ffd.cFileName, filesize.QuadPart);
        }
    } while (FindNextFile(hFind, &ffd) != 0);

    dwError = GetLastError();
    if (dwError != ERROR_NO_MORE_FILES)
    {
        print_error(TEXT("FindFirstFile"));
        return dwError;
    }

    FindClose(hFind);
    return ERROR_SUCCESS;
}