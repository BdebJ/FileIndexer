#include "os_includes.h"
#include "utility.h"
#include "indexer.h"

int _tmain(int argc, TCHAR* argv[])
{
    DWORD start_time = GetTickCount();
    TCHAR directory_path[MAX_PATH];
    size_t length_of_arg;
    DWORD dwError = 0;

    if (argc != 2)
    {
        _tprintf(TEXT("\nUsage: %s <directory name>\n"), argv[0]);
        return (-1);
    }

    // Check that the input path + 3 is not longer than MAX_PATH. Three characters are for the "\*" plus NULL appended below.
    StringCchLength(argv[1], MAX_PATH, &length_of_arg);
    if (length_of_arg > (MAX_PATH - 3))
    {
        _tprintf(TEXT("\nDirectory path is too long.\n"));
        return (-1);
    }

    _tprintf(TEXT("\nTarget directory is %s\n\n"), argv[1]);
    StringCchCopy(directory_path, MAX_PATH, argv[1]);



    dwError = index_directory(directory_path, 0);
    DWORD elapsed_time = GetTickCount() - start_time;
    _tprintf(TEXT("Time elapsed: %u milliseconds\n"), elapsed_time);
    return dwError;
}